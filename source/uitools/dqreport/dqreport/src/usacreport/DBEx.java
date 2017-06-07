package usacreport;

import java.util.*;
import java.text.*;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class DBEx {
   private static final String tabPerDay[]={"nothing"};
   private static final String tabPerMonth[]={"yc","kwh","dayyc","daykwh","开关变位统计表","开关遥控统计表","电力历史事项表","操作历史事项表"};
   private static final String tabPerYear[]={"monthyc","monthkwh"};

   //判断一个SQL语句是否是聚合函数,目前系统聚合函数在分表时仅返回第一次查询结果
   private static final boolean IsAggregationSQL(String Sql){
      String AggregationDef[]={"avg","max","min","sum","count"};
      int pos1,pos2;
      Sql=Sql.toLowerCase();
      pos1=Sql.indexOf("select");
      pos2=Sql.indexOf("from");
      String strTmp=Sql.substring(pos1+6,pos2);
      for(int i=0;i<AggregationDef.length;i++){
        if(strTmp.indexOf(AggregationDef[i])>=0){
           return true;
        }
     }
     return false;
   }

   private static final String GetAggregationStr(String Sql){
      String AggregationDef[]={"avg","max","min","sum","count"};
      String retStr="";
      int pos1, pos2;
      Sql = Sql.toLowerCase();
      pos1 = Sql.indexOf("select");
      pos2 = Sql.indexOf("from");
      String strTmp = Sql.substring(pos1 + 6, pos2);
      for (int i = 0; i < AggregationDef.length; i++) {
         if(strTmp.indexOf(AggregationDef[i])>=0){
            retStr=AggregationDef[i];
            break;
       }
      }
      return retStr;
   }


   public static double SqlReadRecorder(String dbname,
                                        String sql,
                                        int type) {
      return usacreport.FrmSacReport.f1jdll.SqlReadRecorder(dbname.getBytes(),
          sql.getBytes(),type);
   }

   //获得一个记录集,注意表名包含了日期，不自动获得。
   //返回记录集中字段个数，表名需要日期。
   public static int ReadRecorders(String dbname,String tablename,
                                   String fieldname,String where,
                                   String orderfn,Vector content,boolean bDistinct)  {
      int returnVal=0;
      int []icount = new int[1];
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      orderfn.getBytes(),icount,bDistinct);
      if(returnv>0){
         byte [][] buffer = new byte[0][0];
         buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
         int ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
         content.removeAllElements();
         for(int i=0;i<returnv;i++){
            String tmpStr=new String(buffer[i]);
            tmpStr=tmpStr.trim();
            content.addElement(tmpStr);
         }
         returnVal=icount[0];
      }
      else{
         returnVal=0;
      }
      return returnVal;
   }

  //直接从SQL语句中获得结果集，sql是可以被直接执行的语句
  public static int SqlReadRecorders(String dbname,String sql,Vector content){
     int colCnt;
     int[] fieldcount=new int[1];
     int returnv = usacreport.FrmSacReport.f1jdll.SqlReadRecorders(dbname.getBytes(),sql.getBytes(),fieldcount);
     content.clear();
     if(returnv>0){
        byte [][] buffer = new byte[0][0];
        buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
        content.removeAllElements();
        for(int i=0;i<returnv;i++){
           String tmpStr=new String(buffer[i]);
           tmpStr=tmpStr.trim();
           content.addElement(tmpStr);
        }
        colCnt=fieldcount[0];
      //  System.out.println(fieldcount);
       // System.out.println(fieldcount[0]);
     }
     else{
        colCnt=0;
     }
     return colCnt;
  }
   //直接由SQL获得结果集，需要注意的是这里的sql是能够被执行的。
   //仅仅需要的是替换表名和注意开始和结束时间。
   //返回数据集的列个数。记录个数直接通过结果集个数/列个数获得。
   public static int SqlReadRecorders(String dbname,
                                      String sql,
                                      Calendar cal_dtBegin,
                                      Calendar cal_dtEnd,
                                      Vector content) {
      int returnColCnt=0;
      String realTabName="",tablename="",tabletime;

      Calendar cal_dtS=Calendar.getInstance(TimeZone.getDefault());
      Calendar cal_dtE=Calendar.getInstance(TimeZone.getDefault());
      cal_dtS.setTime(cal_dtBegin.getTime());
      cal_dtE.setTime(cal_dtEnd.getTime());
      if(cal_dtS.getTimeInMillis()>cal_dtE.getTimeInMillis())
         return returnColCnt;
      content.removeAllElements();


      //从SQL中获得表名
      String word[] = sql.split(" ");
      for (int i = 0; i < word.length; i++) {
         word[i] = word[i].trim();
      }

      for (int i = 0; i < word.length; i++) {
         if (word[i].compareToIgnoreCase("") == 0)
            continue;
         if(word[i].toLowerCase().indexOf("from")>=0){
            tablename = word[i + 1];
            break;
         }
      }
      if(tablename.length()==0)  //没有表名失败。
         return 0;

      //判断表类型，如果不需要生成名字，就直接获得结果数据
      int tabStyle = GetTableDateStyle(tablename);
      if (tabStyle == 0) {
         return SqlReadRecorders(dbname, sql, content);
      }
      //sql中以包含完整表名，这里强制策略为去掉表名中的日期，有系统自动生成。
      int spos,tabnameLen=0;
      if (tablename.matches("\\S*\\d{4,8}")) {
         tabnameLen=tablename.length();
         tablename = tablename.replaceFirst("\\d{4,8}", "");
      }
      else
         tabnameLen=tablename.length();


      StringBuffer orgStrBuf=new StringBuffer(sql);
      spos=orgStrBuf.indexOf(tablename);
      //根据表类型，从开始时间到结束时间来循环表周期，生成替换具体表名，获得结果集合。
      //如果日期一致,则直接获得结果返回
      if (cal_dtS.get(cal_dtS.YEAR) == cal_dtE.get(cal_dtE.YEAR) &&
          cal_dtS.get(cal_dtS.MONTH) == cal_dtE.get(cal_dtE.MONTH) &&
          cal_dtS.get(cal_dtS.DAY_OF_MONTH) ==
          cal_dtE.get(cal_dtE.DAY_OF_MONTH)) {
         if (tabStyle == 1) { //每日表
            SimpleDateFormat form = new SimpleDateFormat("yyyyMMdd");
            tabletime = form.format(cal_dtS.getTime());
            realTabName = tablename + tabletime;
         }
         else if (tabStyle == 2) {
            SimpleDateFormat form = new SimpleDateFormat("yyyyMM");
            tabletime = form.format(cal_dtS.getTime());
            realTabName = tablename + tabletime;
         }
         else if (tabStyle == 3) {
            SimpleDateFormat form = new SimpleDateFormat("yyyy");
            tabletime = form.format(cal_dtS.getTime());
            realTabName = tablename + tabletime;
         }
         orgStrBuf.replace(spos, spos+tabnameLen, realTabName);
         sql = orgStrBuf.toString();
         int colCnt = SqlReadRecorders(dbname, sql, content);
         returnColCnt=colCnt;
      }
      else if(tabStyle==1){//处理日表，目前只有新疆阿克苏采用的方式
         //注意这里处理where时的时间范围并没有改变，应该不影响数据库查询结果。
         Vector tmpContent = new Vector();
         while(!(cal_dtS.get(cal_dtS.YEAR) == cal_dtE.get(cal_dtE.YEAR) &&
          cal_dtS.get(cal_dtS.MONTH) == cal_dtE.get(cal_dtE.MONTH) &&
          cal_dtS.get(cal_dtS.DAY_OF_MONTH) ==
          cal_dtE.get(cal_dtE.DAY_OF_MONTH))){
            SimpleDateFormat form = new SimpleDateFormat("yyyyMMdd");
            tabletime = form.format(cal_dtS.getTime());
            realTabName = tablename + tabletime;
            StringBuffer tmpStrBuf=new StringBuffer(orgStrBuf.toString());
            tmpStrBuf.replace(spos, spos+tabnameLen, realTabName);
            sql=tmpStrBuf.toString();
            int colCnt=SqlReadRecorders(dbname,sql,tmpContent);
            if(colCnt>0){
               returnColCnt=colCnt;
               content.addAll(tmpContent);
            }
            cal_dtS.add(cal_dtS.DAY_OF_MONTH,1);
         }
      }
      else if(tabStyle==2){  //处理月表
         Vector tmpContent = new Vector();
         int dateS=cal_dtS.get(cal_dtS.YEAR)*100+cal_dtS.get(cal_dtS.MONTH)+1;
         int dateE=cal_dtE.get(cal_dtE.YEAR)*100+cal_dtE.get(cal_dtE.MONTH)+1;
         while (dateS <=  dateE) {
            //合成表名
            SimpleDateFormat form = new SimpleDateFormat("yyyyMM");
            tabletime = form.format(cal_dtS.getTime());
            realTabName = tablename + tabletime;
            StringBuffer tmpStrBuf=new StringBuffer(orgStrBuf.toString());
            tmpStrBuf.replace(spos, spos+tabnameLen, realTabName);
            sql=tmpStrBuf.toString();
            int colCnt=SqlReadRecorders(dbname,sql,tmpContent);
            if(PreProcessor.SQL_DEBUG==true)
               PreProcessor.WriteLogFile("实际执行的SQL:"+sql);
            if (colCnt > 0) {
               returnColCnt = colCnt;
               content.addAll(tmpContent);
            }
            cal_dtS.add(cal_dtS.MONTH,1);
            dateS=cal_dtS.get(cal_dtS.YEAR)*100+cal_dtS.get(cal_dtS.MONTH)+1;

         } //end while
         //多表结合查询,如果涉及到多表,则仅从开始时间点的表中查询一次聚合函数的结果
         //090908修改正常查询结果，在返回时将多个结果处理为一个结果
         //目前仅处理sum,count,max,min聚合，对于avg没有好的办法。
         if (IsAggregationSQL(sql)) {
            String tmpStr = GetAggregationStr(sql);
            if (tmpStr.compareToIgnoreCase("sum(") == 0 ||
                tmpStr.compareToIgnoreCase("count(") == 0) {  //hongxiang修改,表的域名字符串带有count的不能统计
       // if (tmpStr.compareToIgnoreCase("sum(") == 0){
               tmpContent.clear();
               double sum = 0;
               for (int i = 0; i < content.size(); i++) {
                  sum += Double.parseDouble(content.get(i).toString());
               }
               content.clear();
               content.addElement(String.valueOf(sum));
            }
            else if (tmpStr.compareToIgnoreCase("max") == 0) {
            }
            else if (tmpStr.compareToIgnoreCase("min") == 0) {
            }
         }
      }
      else if(tabStyle==3) { //处理年表
         int syear,eyear;
         syear=cal_dtS.get(cal_dtS.YEAR);
         eyear=cal_dtE.get(cal_dtE.YEAR);
         Vector tmpContent = new Vector();
         while (syear <= eyear) {
            SimpleDateFormat form = new SimpleDateFormat("yyyy");
            tabletime = form.format(cal_dtS.getTime());
            realTabName = tablename + tabletime;
            StringBuffer tmpStrBuf=new StringBuffer(orgStrBuf.toString());
            tmpStrBuf.replace(spos, spos+tabnameLen, realTabName);
            sql = tmpStrBuf.toString();
            int colCnt = SqlReadRecorders(dbname, sql, content);
            if(PreProcessor.SQL_DEBUG==true)
               PreProcessor.WriteLogFile("实际执行的SQL:"+sql);
            if (colCnt > 0) {
               returnColCnt=colCnt;
               content.addAll(tmpContent);
            }
            cal_dtS.add(cal_dtS.YEAR,1);
            syear++;
         }
      }
      return returnColCnt;
   }


   public static final int GetTableDateStyle(String tablename){
      for(int i=0;i<tabPerDay.length;i++){
         if(tablename.matches(tabPerDay[i]+"\\d*")){
            return 1;
         }
      }
      for(int i=0;i<tabPerMonth.length;i++){
         if(tablename.matches(tabPerMonth[i]+"\\d*")){
            return 2;
         }
      }
      for(int i=0;i<tabPerYear.length;i++){
        if(tablename.matches(tabPerYear[i]+"\\d*")){
           return 3;
        }
      }
      return 0;
   }

   //dateStyle
   //0:数据库中历史库时间标准类型,date为int型，转换后为格式化后的日期串
   //1:date为Calendar型，转换后为数值型的串.
   //dateFormat：SimpleDateFormat支持的类型，为标准的日期格式化串支持[yyyy-MM-dd]格式，如果是Calendar类型，支持[yyyy-01-23]格式
   //返回值，如果失败返回空串。
   public static final String ConvertDate(Object date,String dateFormat,String dateType){
      String returnStr = "";
      Calendar tmpCal = Calendar.getInstance(TimeZone.getDefault());
      try {
         if (date instanceof Integer || date instanceof String) {
            long ltemp = Integer.parseInt(String.valueOf(date));
            tmpCal.set(Calendar.ZONE_OFFSET, 28800000);
            ltemp = ltemp * (long) 86400000.0;
            tmpCal.setTime(new Date(ltemp));
         }
         else if (date instanceof Calendar) {
            tmpCal.setTime( ( (Calendar) date).getTime());
         }
         if (dateFormat.equalsIgnoreCase("WW")) {
            switch (tmpCal.get(Calendar.DAY_OF_WEEK)) {
               case 1:
                  returnStr = "星期日";
                  break;
               case 2:
                  returnStr = "星期一";
                  break;
               case 3:
                  returnStr = "星期二";
                  break;
               case 4:
                  returnStr = "星期三";
                  break;
               case 5:
                  returnStr = "星期四";
                  break;
               case 6:
                  returnStr = "星期五";
                  break;
               case 7:
                  returnStr = "星期六";
                  break;
               default:
            }
         }
         else {
            SimpleDateFormat form = new SimpleDateFormat(dateFormat);
            returnStr = form.format(tmpCal.getTime());
         }
      }
      catch (Exception ex) {
         returnStr = "";
      }

      return returnStr;
   }


   public static final String ConvertTime(Object time,String timeFormat,String timeType){
      String returnStr="";
      Calendar tmpCal=Calendar.getInstance(TimeZone.getDefault());
      tmpCal.set(2000,0,1,0,0,0);
      try{
         if (time instanceof Integer || time instanceof String){
            long ltemp = (long) Double.parseDouble(String.valueOf(time));
            if (timeType.compareToIgnoreCase("") == 0) { //缺省类型
               int day = (int) (ltemp / 1440);
               int hour = (int) (ltemp - day * 1440) / 60;
               int minute = (int) (ltemp - day * 1440) % 60;
               //-----广州测试添加特殊处理  -hongxiang
               if(minute<60 && hour ==0 && day==0){
                  if (minute==5){
                     returnStr="0"+time.toString();
                  }
                  else{
                     returnStr = time.toString();
                  }
                  returnStr= "00:"+returnStr;
                  return returnStr;
               }
               //------广州测试添加特殊处理
               tmpCal.set(tmpCal.MINUTE, minute);
               tmpCal.set(tmpCal.HOUR_OF_DAY, hour);
               tmpCal.add(tmpCal.DAY_OF_MONTH, day);
           }
           else if(timeType.compareToIgnoreCase("intSecond")==0){
              int nSecond = (int) ltemp % 60;
              tmpCal.set(tmpCal.SECOND, nSecond);
              ltemp /= 60;
              int day = (int) (ltemp / 1440);
              int hour = (int) (ltemp - day * 1440) / 60;
              int minute = (int) (ltemp - day * 1440) % 60;
              tmpCal.set(tmpCal.MINUTE, minute);
              tmpCal.set(tmpCal.HOUR_OF_DAY, hour);
              tmpCal.add(tmpCal.DAY_OF_MONTH, day);
              //特定格式化，自己处理。
              if(timeFormat.compareToIgnoreCase("mm")==0){
                 returnStr=String.valueOf(ltemp);
                 return returnStr;
              }
           }
        }
        else if (time instanceof Calendar) {
           tmpCal.setTime(((Calendar)time).getTime());
        }
        SimpleDateFormat form = new SimpleDateFormat(timeFormat);
        returnStr = form.format(tmpCal.getTime());
      }
      catch(Exception ex){
      }
      return returnStr;
   }
}
