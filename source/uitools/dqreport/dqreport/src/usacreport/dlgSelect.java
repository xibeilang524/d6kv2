package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import com.borland.dbswing.*;
import java.awt.event.*;
import java.util.Calendar;
import java.util.TimeZone;
//import ipworks.*;
import java.io.File;
import java.io.FileOutputStream;
import java.util.Vector;
import java.util.Date;
import java.util.*;
import java.text.*;
import java.awt.BorderLayout;
import java.awt.Font;
import javax.swing.border.EmptyBorder;
import java.awt.Dimension;
import com.f1j.util.*;

/**
 * Description: 报表工具
 * Company: daqo
 * 生成报表时填充数据
 * version 1.0
 */

class sqlConstantDef {
   public static String stimeSignStr = "begin";
   public static String etimeSignStr = "end";
   public static String ntimeSignStr = "now";
   public static String sSplitStr = "[";
   public static String eSplitStr = "]";
   public static String keySignSplitStr = ":";

   public static final char DayChar='d';
   public static final char MonthChar='M';
   public static final char YearChar='y';


};

public class dlgSelect
    extends JDialog {
   private JPanel panel1 = new JPanel();
   private XYLayout xYLayout1 = new XYLayout();
   private JLabel jLabel1 = new JLabel();
   private JLabel jLabel2 = new JLabel();
   private JLabel jLabel3 = new JLabel();
   private JButton jButOK = new JButton();
   private JComboBox jComyear = new JComboBox();
   private JComboBox jCommon = new JComboBox();
   private JComboBox jComday = new JComboBox();
   private JButton jButCancel = new JButton();
   public  Calendar cal_dt = Calendar.getInstance(TimeZone.getDefault());
   private Calendar cal_dt1 = Calendar.getInstance(TimeZone.getDefault());
   private Calendar cal_dt2 = Calendar.getInstance(TimeZone.getDefault());
   private String file_name = "";
   private String g_year = "";
   private String g_mon = "";
   private String g_day = "";
   private String g_hour="";
   //hongxiang 广东测试
   private int g_ghour=0;
   private int g_gweek=0;
   private int g_gjd=0;
   //hongxiang
   public String in_filenameid = "";
   public String out_filename = "";
   public String print_type="";
   private String cmd_linesel = "";
   private String hdb_name = "modeldb";
   private boolean b_first = true;
   private PrintThread pt = null;
   public static Vector vectorprint = new Vector();
   private int vecid = 0;
   public static boolean threadstat = true;
   public static String g_selfilename = "";
   //hongxiang ---广州测试记录,报表标志 1 时报表 2 周报表 3 季报表
   private JLabel jLabel4 = new JLabel();
   public static boolean bData = false;
   private boolean mon_delrowcol = true; //判断月表是否要删除行或者列
   public static Vector veccontent = new Vector(); //用于临时保存需要在表格中显示的数据，方便处理用。
   private String s_cellvalue = "";
   JComboBox jComHour = new JComboBox();
   JLabel jlblHour = new JLabel();
   //hongxiang  广州测试
   JComboBox jComGHour = new JComboBox();
   JLabel jlblGHour = new JLabel();
   JComboBox jComGWeek = new JComboBox();
   JLabel jlblGWeek = new JLabel();
   JComboBox jComJD = new JComboBox();
   JLabel jlblJD = new JLabel();
   public dlgSelect(Frame frame, String title, boolean modal) {
      super(frame, title, modal);
      try {
         jbInit();
         pack();
      }
      catch (Exception ex) {
         ex.printStackTrace();
      }
   }

   public dlgSelect() {
      this(null, "", false);
   }

   private void jbInit() throws Exception {
      cal_dt.set(Calendar.ZONE_OFFSET, 28800000);
      cal_dt1.set(Calendar.ZONE_OFFSET, 28800000);
      cal_dt2.set(Calendar.ZONE_OFFSET, 28800000);

      panel1.setLayout(xYLayout1);
       panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
      jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel1.setText("月");
      jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel2.setText("年");
      jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel3.setText("日期");
      jButOK.setFont(new java.awt.Font("Dialog", 0, 12));
      jButOK.setText("生成报表");
      jButOK.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButOK_actionPerformed(e);
         }
      });
      jButCancel.setFont(new java.awt.Font("Dialog", 0, 12));
      jButCancel.setText("退出");
      jButCancel.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButCancel_actionPerformed(e);
         }
      });
      jCommon.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jCommon_actionPerformed(e);
         }
      });
      jComyear.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComyear_actionPerformed(e);
         }
      });
      jComday.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComday_actionPerformed(e);
         }
      });

      jComHour.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e){
            jComHour_actionPerformed(e);
         }
      });

      jLabel4.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jLabel4.setText("日");
      jComyear.setFont(new java.awt.Font("SansSerif", 0, 12));
      jCommon.setFont(new java.awt.Font("SansSerif", 0, 12));
      jComday.setFont(new java.awt.Font("SansSerif", 0, 12));
      jComHour.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jlblHour.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      //广州测试
      jComGHour.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jlblGHour.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jComGWeek.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jlblGWeek.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jComJD.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      jlblJD.setFont(new java.awt.Font("SansSerif", Font.PLAIN, 12));
      if(usacreport.FrmSacReport.reportflag ==1)
      {
         jComGHour.setVisible(true);
         jlblGHour.setVisible(true);
         jComGWeek.setVisible(false);
         jlblGWeek.setVisible(false);
         jComJD.setVisible(false);
         jlblJD.setVisible(false);

      }
      else if(usacreport.FrmSacReport.reportflag==2){
         jComGWeek.setVisible(true);
         jlblGWeek.setVisible(true);
         jComGHour.setVisible(false);
         jlblGHour.setVisible(false);
         jComJD.setVisible(false);
         jlblJD.setVisible(false);
      }
      else if (usacreport.FrmSacReport.reportflag == 3) {
         jComJD.setVisible(true);
         jlblJD.setVisible(true);
         jComGHour.setVisible(false);
         jlblGHour.setVisible(false);
         jComGWeek.setVisible(false);
         jlblGWeek.setVisible(false);

      }
      else{
         jComGHour.setVisible(false);
         jlblGHour.setVisible(false);
         jComGWeek.setVisible(false);
         jlblGWeek.setVisible(false);
         jComJD.setVisible(false);
         jlblJD.setVisible(false);
      }


      if(!PreProcessor.UseSelHourTime)
      {
         jComHour.setVisible(false);
         jlblHour.setVisible(false);
      }
      else
      {
         jComHour.setVisible(true);
         jlblHour.setVisible(true);
      }
      jlblHour.setText("时");
      jlblGHour.setText("时");
      jlblGWeek.setText("周");
      jlblJD.setText("季");
    //  panel1.setMinimumSize(new Dimension(404, 200));
    //  panel1.setPreferredSize(new Dimension(200, 136));
      panel1.setToolTipText("");
      panel1.add(jlblHour, new XYConstraints(381, 24, 23, 25));
      panel1.add(jlblGHour, new XYConstraints(381, 24, 23, 25));
    //  panel1.add(jlblGWeek, new XYConstraints(381, 24, 23, 25));
      panel1.add(jlblJD, new XYConstraints(381, 24, 23, 25));
      panel1.add(jComyear, new XYConstraints(54, 17, 85, 25));
      panel1.add(jCommon, new XYConstraints(161, 17, 54, 25));
      panel1.add(jComday, new XYConstraints(243, 17, 58, 25));
      panel1.add(jComHour, new XYConstraints(322, 18, 51, 24));
      panel1.add(jComGHour, new XYConstraints(322, 18, 51, 24));
    //  panel1.add(jComGWeek, new XYConstraints(322, 18, 51, 24));
      panel1.add(jComJD, new XYConstraints(322, 18, 51, 24));
      panel1.add(jLabel4, new XYConstraints(306, 24, 23, -1));
      panel1.add(jLabel1, new XYConstraints(227, 24, 19, -1));
      panel1.add(jLabel2, new XYConstraints(143, 24, 19, -1));
      panel1.add(jLabel3, new XYConstraints(18, 23, 40, -1));
      panel1.add(jButOK, new XYConstraints(52, 63, 94, -1));
      panel1.add(jButCancel, new XYConstraints(212, 64, 88, -1));
      this.getContentPane().add(panel1, java.awt.BorderLayout.CENTER);
      if (usacreport.AppSacReport.s_cmdline.length() != 0) {
         if(PreProcessor.CMDLINE_DEBUG){
            PreProcessor.WriteLogFile(usacreport.AppSacReport.s_cmdline);
         }
         cmd_linesel = usacreport.AppSacReport.s_cmdline;
//      or
         int pos = cmd_linesel.lastIndexOf(",");
         if (pos < 0) {
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }

         //输出文件名称
         //lcg20090219考虑到目录行最后有\的情况，修改
         out_filename = cmd_linesel.substring(pos + 1, cmd_linesel.length());
         cmd_linesel = cmd_linesel.substring(0, pos);
         pos = cmd_linesel.lastIndexOf(",");
         if (pos < 0) {
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }

         if (PreProcessor.UseSelHourTime) {
            //时
            g_hour = cmd_linesel.substring(pos + 1, cmd_linesel.length());
            cmd_linesel = cmd_linesel.substring(0, pos);
            pos = cmd_linesel.lastIndexOf(",");
            if (pos < 0) {
               usacreport.FrmSacReport.f1jdll.closedb();
               System.exit(1);
            }
         }
         else
            if(PreProcessor.PWTEST)
               g_hour="21";
            else
               g_hour="0";
         //-------hongxiang

         //天
         g_day = cmd_linesel.substring(pos + 1, cmd_linesel.length());
         cmd_linesel = cmd_linesel.substring(0, pos);
         pos = cmd_linesel.lastIndexOf(",");
         if (pos < 0) {
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }

         //月
         g_mon = cmd_linesel.substring(pos + 1, cmd_linesel.length());
         cmd_linesel = cmd_linesel.substring(0, pos);
         pos = cmd_linesel.lastIndexOf(",");
         if (pos < 0) {
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }
         //年
         g_year = cmd_linesel.substring(pos + 1, cmd_linesel.length());

         if (cmd_linesel.length() > 0) {
            pos = cmd_linesel.lastIndexOf(",");
            cmd_linesel = cmd_linesel.substring(0, pos);
            pos = cmd_linesel.lastIndexOf(",");
            print_type=cmd_linesel.substring(pos + 1, cmd_linesel.length());
            cmd_linesel = cmd_linesel.substring(0, pos);
         }
         else {
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }

         if (cmd_linesel.length() > 0) {
            //类型
            if (cmd_linesel.substring(0, 1).compareTo("\\") == 0) {
               in_filenameid = cmd_linesel.substring(1, pos); //\1,2004
            }
            else {
               if (PreProcessor.SpecialCmdLine) {
                  in_filenameid = cmd_linesel.substring(1, pos); //lcg 20090219
               }
               else {
                  in_filenameid = cmd_linesel.substring(0, pos); //1,2004
               }
            }
         }
         else {
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }
         if(PreProcessor.CMDLINE_DEBUG){
            PreProcessor.WriteLogFile("命令行:" + usacreport.AppSacReport.s_cmdline);
            PreProcessor.WriteLogFile("解析:" + "类型=" + in_filenameid + "，文件=" + out_filename + "，日期=" +
                g_year + g_mon + g_day);
         }

         usacreport.FrmSacReport.babs = true;

         String dbname = "modeldb";
         String tablename = "webRptName";
         String fieldname = "RptName";
         String where = "", order = "";

         where = "webID = " + in_filenameid;

         int[] icount = new int[1];
         int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
             getBytes(), tablename.getBytes(), fieldname.getBytes(),
             where.getBytes(),
             order.getBytes(), icount, true);

         if (returnv > 0) {
            byte[][] buffer = new byte[0][0];
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            String strtemp = new String(buffer[0]);
            in_filenameid = strtemp.trim();
         }
         else {
            if(PreProcessor.CMDLINE_DEBUG)
               PreProcessor.WriteLogFile("webRptName查询失败退出!");
            usacreport.FrmSacReport.f1jdll.closedb();
            System.exit(1);
         }
         if(PreProcessor.CMDLINE_DEBUG==true){
            PreProcessor.WriteLogFile("执行fill_calcmd");
         }
         fill_calcmd();
         if(PreProcessor.CMDLINE_DEBUG==true){
           PreProcessor.WriteLogFile("执行data_proc");
         }
         data_proc();
         if(PreProcessor.CMDLINE_DEBUG==true){
               PreProcessor.WriteLogFile("data_proc过程_结束处理报表,进程将退出");
         }
         usacreport.FrmSacReport.f1jdll.closedb();
         System.exit(1);
      }
      int i, j;
      try {
         b_first = false;
         for (i = 2000; i <= 2030; i++) {
            jComyear.addItem(Integer.toString(i));
         }
         jComyear.setSelectedIndex(0);
         for (i = 1; i <= 12; i++) {
            jCommon.addItem(Integer.toString(i));
         }
         jCommon.setSelectedIndex(0);
         for(i=0;i<24;i++){
            jComHour.addItem(Integer.toString(i));
         }
         jComHour.setSelectedIndex(0);
         for (i = 0; i < 24; i++) {
            jComGHour.addItem(Integer.toString(i));
         }
         jComGHour.setSelectedIndex(0);
         for (i = 1; i < 7; i++) {
            jComGWeek.addItem(Integer.toString(i));
         }
         jComGWeek.setSelectedIndex(0);
         for (i = 1; i < 5; i++) {
            jComJD.addItem(Integer.toString(i));
         }
         jComJD.setSelectedIndex(0);

         dayfill(jCommon.getSelectedIndex() + 1);
         Calendar cal1 = Calendar.getInstance(TimeZone.getDefault());
         cal1.set(Calendar.ZONE_OFFSET, 28800000);
         jComyear.setSelectedIndex(cal1.get(cal1.YEAR) - 2000);
         jCommon.setSelectedIndex(cal1.get(cal1.MONTH));
         jComday.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH) - 1);
      }
      catch (java.lang.IllegalArgumentException jCom) {
      }

      if (pt == null) {
         pt = new PrintThread();
         pt.start();
      }
   }

   public boolean PatchPrint(Calendar sDate,Calendar eDate,int nRptType,String inFilename){
      Calendar cal_dtS = Calendar.getInstance(TimeZone.getDefault());
      Calendar cal_dtE = Calendar.getInstance(TimeZone.getDefault());
      cal_dtS.setTime(sDate.getTime());
      cal_dtE.setTime(eDate.getTime());
      if (cal_dtS.getTimeInMillis() > cal_dtE.getTimeInMillis()) {
         JOptionPane.showMessageDialog(this,"开始时间小于结束时间，请重新选择!","错误提示",JOptionPane.ERROR_MESSAGE);
         return false;
      }
      //日
      if(nRptType==1){
         Date d1=cal_dtS.getTime();
         Date d2=cal_dtE.getTime();
         long nDays=d2.getTime()/86400000 -d1.getTime()/86400000 ;
         if(nDays>=30)
         {
            JOptionPane.showMessageDialog(this,"选择的时间段大于31天，请重新选择!","错误提示",JOptionPane.ERROR_MESSAGE);
            return false;
         }
         //在结束日期上加1天，使日循环时能够执行到结束日那天.
         cal_dtE.add(cal_dtE.DAY_OF_MONTH,1);
         while(!(cal_dtS.get(cal_dtS.YEAR) == cal_dtE.get(cal_dtE.YEAR) &&
         cal_dtS.get(cal_dtS.MONTH) == cal_dtE.get(cal_dtE.MONTH) &&
         cal_dtS.get(cal_dtS.DAY_OF_MONTH) ==
         cal_dtE.get(cal_dtE.DAY_OF_MONTH))){
            cal_dt = cal_dtS;
            out_filename = "print";
            in_filenameid = inFilename;
            data_proc();
            cal_dtS.add(cal_dtS.DAY_OF_MONTH, 1);
         }
     }
    //月
     else if(nRptType==2){
        Date d1 = cal_dtS.getTime();
        Date d2 = cal_dtE.getTime();
        long nDays = d2.getTime() / 86400000 - d1.getTime() / 86400000;
        if (nDays >= 30*6) {
           JOptionPane.showMessageDialog(this, "选择的时间段大于半年，请重新选择!", "错误提示",
                                         JOptionPane.ERROR_MESSAGE);
           return false;
        }

        int dateS=cal_dtS.get(cal_dtS.YEAR)*100+cal_dtS.get(cal_dtS.MONTH)+1;
        int dateE=cal_dtE.get(cal_dtE.YEAR)*100+cal_dtE.get(cal_dtE.MONTH)+1;
        while (dateS <= dateE) {
           cal_dt = cal_dtS;
           out_filename = "print";
           in_filenameid = inFilename;
           data_proc();
           cal_dtS.add(cal_dtS.MONTH, 1);
           dateS = cal_dtS.get(cal_dtS.YEAR) * 100 + cal_dtS.get(cal_dtS.MONTH) +1;
        }

     }
      return true;
   }

   class PrintThread
       extends Thread {
      public void run() {
         try {
            while (threadstat) {
               Calendar cd = Calendar.getInstance(TimeZone.getDefault());
               cd.set(Calendar.ZONE_OFFSET, 28800000);
               String stime = Integer.toString(cd.get(cd.HOUR_OF_DAY) * 60 +cd.get(cd.MINUTE));
               String dtime = Integer.toString(cd.get(cd.DAY_OF_MONTH)*3600+cd.get(cd.HOUR_OF_DAY) * 60 + cd.get(cd.MINUTE));
               for (int i = 0; i < vectorprint.size(); i += 5) {
                  if (stime.compareTo( (String) vectorprint.get(i + 1)) == 0 &&
                      "0".compareTo( (String) vectorprint.get(i + 4)) == 0) {
                     System.out.println("日报表可以打印了!");
                     cal_dt = cd;
                     out_filename = "print";
                     vecid = i;
                     in_filenameid = (String) vectorprint.get(i);
                     vectorprint.set(i + 4, "1");//表示已被打印过,防止下次线程进入后再次处理它
                     data_proc();
                  }
                  else if(dtime.compareTo((String)vectorprint.get(i + 1)) == 0 &&
                      "0".compareTo((String) vectorprint.get(i + 4))==0){
                     System.out.println("月报表可以打印了!");
                     cal_dt = cd;
                     out_filename = "print";
                     vecid = i;
                     in_filenameid = (String) vectorprint.get(i);
                     vectorprint.set(i + 4, "1");
                     data_proc();
                  }

                  //报表打印时间和当前时间差1分钟后，设置为可在打印
                  Calendar cd1 = Calendar.getInstance(TimeZone.getDefault());
                  cd1.set(Calendar.ZONE_OFFSET, 28800000);
                  String stime1 = Integer.toString(cd1.get(cd.HOUR_OF_DAY) * 60 +
                      cd.get(cd.MINUTE));
                  String dtime1 = Integer.toString(cd1.get(cd.DAY_OF_MONTH) *
                      3600 + cd.get(cd.HOUR_OF_DAY) * 60 + cd.get(cd.MINUTE));


                     if(Integer.parseInt( (String) vectorprint.get(i +
                         1))>3600)
                     {
                        if (Math.abs(Integer.parseInt(dtime1) -
                                     Integer.parseInt( (String) vectorprint.get(
                            i +
                            1))) >
                            60) {
                           vectorprint.set(i + 4, "0");
                        }
                     }
                     else
                         {
                         if (Math.abs(Integer.parseInt(stime1) -
                                      Integer.parseInt( (String) vectorprint.
                         get(i +
                             1))) >
                             60) {
                        vectorprint.set(i + 4, "0");
                     }

                     }

//                  if ((Integer.parseInt(stime)-Integer.parseInt((String)vectorprint.get(i + 1))) == 1) {
//                        vectorprint.set(i + 4, "0");
//                  }
               }
              // System.out.println("打印进程运行中!");
               Thread.sleep(10); //hongxiang -20120311 修改定时打印会打多次的问题
            }
            pt.interrupt();
            pt = null;
         }
         catch (java.lang.InterruptedException e) {
         }
      }
   }

   void jButCancel_actionPerformed(ActionEvent e) {
      this.hide();
   }

   void jButOK_actionPerformed(ActionEvent e) {
      usacreport.FrmSacReport.g_savemode = false;
      data_proc();
      this.hide();
   }

   void dayfill(int mon) {
      int i;
      jComday.removeAllItems();
      switch (mon) {
         case 1:
         case 3:
         case 5:
         case 7:
         case 8:
         case 10:
         case 12: {
            for (i = 0; i < 31; i++) {
               jComday.addItem(Integer.toString(i + 1));
            }
            break;
         }
         case 4:
         case 6:
         case 9:
         case 11: {
            for (i = 0; i < 30; i++) {
               jComday.addItem(Integer.toString(i + 1));
            }
            break;
         }
         case 2: {
            if ( (jComyear.getSelectedIndex() + 2000) % 4 != 0) {
               for (i = 0; i < 28; i++) {
                  jComday.addItem(Integer.toString(i + 1));
               }
            }
            else if ( (jComyear.getSelectedIndex() + 2000) % 100 != 0) {
               for (i = 0; i < 29; i++) {
                  jComday.addItem(Integer.toString(i + 1));
               }
            }
            else if ( (jComyear.getSelectedIndex() + 2000) % 400 != 0) {
               for (i = 0; i < 28; i++) {
                  jComday.addItem(Integer.toString(i + 1));
               }
            }
            else {
               for (i = 0; i < 29; i++) {
                  jComday.addItem(Integer.toString(i + 1));
               }
            }
         }
      }
      jComday.setSelectedIndex(0);
      fill_cal();
   }

   void jCommon_actionPerformed(ActionEvent e) {
      dayfill(jCommon.getSelectedIndex() + 1);
   }

   /**
    下面处理规律的日报表数据
    参数("数据类型(dh,dd,maxtime,mintime),
         历史表年月格式(200411),
         0(不用),
         0(不用),
         sdate(1970到现在的天数),
         hour(不用),
         行,
         列
       ")
    20101223添加数据越限判断着色功能（使用遥测参数表中的越限和告警标志实现）
    */
   int dh_proc(String type, String name, String tabletime, int begin, int end,
               int nowdate, int selhour, int row, int col) {
      String dbname = hdb_name;
      String tablename;


      int bdir = 0; //纵向
      if (type.compareTo("dh") == 0 || type.compareTo("maxtime") == 0 ||
          type.compareTo("mintime") == 0||type.compareTo("da") == 0) {
         tablename = "yc" + tabletime;
      }
      else {
         tablename = "kwh" + tabletime;
      }
      String fieldname = "time,flag,data";
      String where = "", order = "time";
      // where = "代码='"+name+"'";
      String str_1 = null;
      /* String str_2=null;
       int []icount_1= new int[1];
       int returnv_1 = FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),"遥测参数表".getBytes(),"类型".getBytes(),where.getBytes(),
           order.getBytes(),icount_1,true);

       if(returnv_1 <= 0){
//      System.out.println("读取终端参数表失败!");
         this.hide();
       }else{
         byte [][] buffer = new byte[0][0];
         buffer = new byte[returnv_1][usacreport.ConstantDef.MAX_COLUMN_LEN];
         int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
         str_2 = new String(buffer[0]).trim();
       }*/
      int num = 0;
      if (name.compareTo("SsysjxloadP") == 0 ||
          ( (name.compareTo("SsysjxP") == 0) && (selhour == 51))) {
         where = "name = '" + name + "' and sdate = " + nowdate +
             " and flag = 51 and mod(time,15)=0 and time>0";
         num = 96;
      }
      else {
         //  where = "name = '" + name + "' and sdate = " + Integer.toString(nowdate)
         //   + " and flag < 50 and time in (60,120,180,240,300,360,420,480,540,600,660,720,780,840,900,960,1020,1080,1140,1200,1260,1320,1380,1440)";
         if (type.compareTo("dh") == 0 || type.compareTo("maxtime") == 0 ||
             type.compareTo("mintime") == 0) {
            where = "name = '" + name + "' and sdate = " + nowdate +
                " and flag < 50 and mod(time,60)=0";
            num = 24;
         }
    /*     else if(type.compareTo("da") == 0){  //计划值
            where = "name = '" + name + "' and sdate = " + nowdate +
               " and flag = 51 and mod(time,60)=0";
           num = 24;
         }
     */
         else if (type.compareTo("dd") == 0) {
            where = "name = '" + name + "' and sdate = " + nowdate +
                " and flag < 50 and mod(time,60)=0";
            num = 24;
         }
         else if (type.compareTo("db") == 0) {
            where = "name = '" + name + "' and sdate = " + nowdate +
                " and flag =52 and mod(time,60)=0";
            num = 24;
         }
      }
      if (selhour > 0 && selhour < 51) {
         where = "name = '" + name + "' and sdate = " + nowdate +
             " and flag < 50 and time = " + Integer.toString(selhour * 60);
      }

      String str_temp = "";
      if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
          type.compareTo("db") == 0) {
         try {
            str_temp = usacreport.FrmSacReport.jBook1.getText(row, col + 1).
                trim();
            if (str_temp.indexOf(",") > 0) {
               str_temp = str_temp.substring(str_temp.indexOf(",") + 1,
                                             str_temp.length());
            }
            int dot_pos = 0;
            dot_pos = str_temp.indexOf(",");
            if (dot_pos > 0) {
               str_temp = str_temp.substring(0, dot_pos);
            }
            if (str_temp.compareTo(name) == 0) {
               bdir = 1; //横向
            }
            else {
               bdir = 0; //纵向
            }
         }
         catch (com.f1j.util.F1Exception edir) {
         }
      }

      if (selhour > 0 && selhour < 51) {
         bdir = 2; //小时数据
      }
      //为回写数据做准备
      if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
          type.compareTo("db") == 0) {
         fillcellpara(name, tablename, 0, nowdate, selhour, type, row, col,
                      bdir); //横纵向的填充再处理
      }
      int[] icount = new int[1];
      int i = 0, j = 0;
      int pos = 0;

      if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
          type.compareTo("db") == 0) {
         if (bdir == 1) {
            pos = col;
         }
         else if (bdir == 0) {
            pos = row;
         }
      }
      if (!isintab(tablename)) {
         try {
            if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
                type.compareTo("db") == 0) {
               if (bdir == 0) { //纵
                  for (i = row; i < row + num; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = i;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
               }
               else if (bdir == 1) { //横
                  for (i = col; i < col + num; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = i;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
               }
               else {
                  usacreport.FrmSacReport.jBook1.setText(row, col, "00:00");
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
            else {
               usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
               usacreport.FrmSacReport.jBook1.setCellText("00:00");
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               usac_vec.row = row;
               usac_vec.col = col;
               usac_vec.str_cell = s_cellvalue;
               veccontent.addElement(usac_vec);
            }
            return row;
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }

      int returnv = 0;
      if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
          type.compareTo("db") == 0) {
         returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             order.getBytes(), icount, true);
      }
      else {
         returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             "data".getBytes(), icount, true);
      }

      if (returnv <= 0) {
         try {
            if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
                type.compareTo("db") == 0) {
               if (bdir == 0) { //纵
                  for (i = row; i < row + num; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = i;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                  }
               }
               else if (bdir == 1) { //横
                  for (i = col; i < col + num; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = i;
                     usac_vec.str_cell = s_cellvalue;
                  }
               }
               else {
                  usacreport.FrmSacReport.jBook1.setNumber(row, col, 0.0000);
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
               }
            }
            else {
               usacreport.FrmSacReport.jBook1.setText(row, col, "00:00");
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               usac_vec.row = row;
               usac_vec.col = col;
               usac_vec.str_cell = s_cellvalue;
            }
            veccontent.addElement(s_cellvalue);
            return row;
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      int time_pos = 0, time_temp = 0;
      double m_result = 0.0;
      if (name.compareTo("SsysjxloadP") == 0 ||
          ( (name.compareTo("SsysjxP") == 0) && (selhour == 51))) {
         time_pos = 15;
      }
      else {
         time_pos = 60;
      }
      if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
          type.compareTo("db") == 0) {
         //从历史库中获得数据后开始写入表格
         for (j = 0; j < returnv; j += 3) {
            String str1 = new String(buffer[j]); //time
            String str3 = new String(buffer[j + 1]); //time
            String str2 = new String(buffer[j + 2]); //data
            str1 = str1.trim();
            str2 = str2.trim(); //.substring(0,6);//锁定6位有效数字(包括小数点)
            str3 =str3.trim();
            m_result = Double.parseDouble(str2);
            if (usacreport.FrmSacReport.babs) {
               if (m_result < 0) {
                  m_result = Math.abs(m_result);
               }
            }

            try {
               if (bdir == 2) {
                  usacreport.FrmSacReport.jBook1.setNumber(row, col, m_result);
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
                  break;
               }
               str_1 = Integer.toString(time_pos);
               if (Integer.parseInt(str1) < Integer.parseInt(str_1)) {
                  continue; //如果有重复数据
               }
               while (str_1.compareTo(str1) != 0) { //如果中间缺数据
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  if (bdir == 0) {
                     usacreport.FrmSacReport.jBook1.setNumber(pos, col, 0.0000);
                     usac_vec.row = pos;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, pos, 0.0000);
                     usac_vec.row = row;
                     usac_vec.col = pos;
                     usac_vec.str_cell = s_cellvalue;
                  }
                  veccontent.addElement(usac_vec);
                  pos++;
                  int n = 0;
                  if (name.compareTo("SsysjxloadP") == 0 ||
                      ( (name.compareTo("SsysjxP") == 0) && (selhour == 51))) {
                     n = 15;
                  }
                  else {
                     n = 60;
                  }
                  time_pos += n;
                  str_1 = Integer.toString(time_pos);
               }
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               if (bdir == 1) {
                  //usacreport.FrmSacReport.jBook1.setNumber(row, pos, m_result);
                  if ((str3.compareTo("13") == 0) || (str3.compareTo("12")==0)){
                     com.f1j.ss.CellFormat cellFormat;
                     usacreport.FrmSacReport.jBook1.setActiveCell(row,pos);
                     cellFormat=usacreport.FrmSacReport.jBook1.getCellFormat();
                     cellFormat.setFontBold(true);
                     cellFormat.setFontColor(Color.blue.getRGB());
                     usacreport.FrmSacReport.jBook1.setCellFormat(cellFormat);
                     usacreport.FrmSacReport.jBook1.setNumber(row, pos, m_result);
                  }
                  else
                  {
                     SetCellNumber(row, pos, m_result, name);
                  }
                  usac_vec.row = row;
                  usac_vec.col = pos;
                  usac_vec.str_cell = s_cellvalue;
               }
               else {
                  //usacreport.FrmSacReport.jBook1.setNumber(pos, col, m_result);
                  if ( (str3.compareTo("13") == 0) ||
                      (str3.compareTo("12") == 0)) {
                     com.f1j.ss.CellFormat cellFormat;
                     usacreport.FrmSacReport.jBook1.setActiveCell(pos, col);
                     cellFormat = usacreport.FrmSacReport.jBook1.getCellFormat();
                     cellFormat.setFontBold(true);
                     cellFormat.setFontColor(Color.blue.getRGB());
                     usacreport.FrmSacReport.jBook1.setCellFormat(cellFormat);
                     usacreport.FrmSacReport.jBook1.setNumber(pos, col,
                         m_result);
                  }
                  else {

                     SetCellNumber(pos, col, m_result, name);
                  }
                  usac_vec.row = pos;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
               }
               veccontent.addElement(usac_vec);
               pos++;
               int n = 0;
               if (name.compareTo("SsysjxloadP") == 0 ||
                   ( (name.compareTo("SsysjxP") == 0) && (selhour == 51))) {
                  n = 15;
               }
               else {
                  n = 60;
               }
               time_pos += n;
            }
            catch (com.f1j.util.F1Exception e2) {
            }
         }
      }
      else {
         try {
            int timemode = 0;
            if (type.compareTo("maxtime") == 0) {
               String str1 = new String(buffer[returnv - 3]); //time
               str1 = str1.trim();
               timemode = Integer.parseInt(str1);
            }
            else {
               String str1 = new String(buffer[0]); //time
               str1 = str1.trim();
               timemode = Integer.parseInt(str1);
            }
            int hour = 0, min = 0;
            String str_hour = "", str_min = "";
            hour = timemode / 60;
            min = timemode % 60;
            if (hour < 10) {
               str_hour = "0" + Integer.toString(hour);
            }
            else {
               str_hour = Integer.toString(hour);
            }
            if (min < 10) {
               str_min = "0" + Integer.toString(min);
            }
            else {
               str_min = Integer.toString(min);
            }
            usacreport.FrmSacReport.jBook1.setText(row, col,
                str_hour + ":" + str_min);
            usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
            usac_vec.row = row;
            usac_vec.col = col;
            usac_vec.str_cell = s_cellvalue;
            veccontent.addElement(usac_vec);
         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }

      if (type.compareTo("dh") == 0 || type.compareTo("dd") == 0 ||
          type.compareTo("db") == 0) {
         try {
            if (bdir == 1) {
               if (pos < col + num) {
                  for (i = pos; i < col + num; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = i;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
               }
            }
            else if (bdir == 0) {
               if (pos < row + num) {
                  for (i = pos; i < row + num; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = i;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
               }
            }
         }
         catch (com.f1j.util.F1Exception eei) {
         }
      }

      try {
         usacreport.FrmSacReport.jBook1.recalc();
      }
      catch (com.f1j.util.F1Exception e3) {
      }
      return row;
   }

   //遥测量越限判断使用颜色填充单元格
   private static String lastYcCode="";  //用于保存填充的上一次判断遥测代码，如果一致则不必从数据库中获得越限数据。
   private static Vector vectOverLimPara=new Vector();
   private int SetCellNumber(int row,int col,double val,String yccode)
   {
      if(lastYcCode.compareToIgnoreCase(yccode)!=0)
      {
         vectOverLimPara.clear();
         int    colCnt;
         String strSql="";
         strSql="select 代码,描述,设备代码,处理越限级别,警告下限,警告上限,警报下限,警报上限 from 遥测参数表 where 代码 ='" +
             yccode + "' and 处理越限级别 <>0";
         colCnt=DBEx.SqlReadRecorders("modeldb",strSql,vectOverLimPara);
      }
      if(vectOverLimPara.size()>0)
      {
         try {
            com.f1j.ss.CellFormat cellFormat;
            double level1[] = new double[2], level2[] = new double[2];
            level1[0] = Double.parseDouble(vectOverLimPara.get(4).toString());
            level1[1] = Double.parseDouble(vectOverLimPara.get(5).toString());
            level2[0] = Double.parseDouble(vectOverLimPara.get(6).toString());
            level2[1] = Double.parseDouble(vectOverLimPara.get(7).toString());
            usacreport.FrmSacReport.jBook1.setActiveCell(row,col);
            cellFormat=usacreport.FrmSacReport.jBook1.getCellFormat();
            if (val >= level1[0] && val <= level1[1]) { //正常
               usacreport.FrmSacReport.jBook1.setNumber(row, col, val);
            }
            else if((val<level1[0] && val>=level2[0]) || (val>level1[1] && val<=level2[1]))  //一级
            {
               cellFormat.setFontBold(true);
               cellFormat.setFontColor(Color.yellow.getRGB());
               usacreport.FrmSacReport.jBook1.setCellFormat(cellFormat);
               usacreport.FrmSacReport.jBook1.setNumber(row, col, val);
            }
            else
            {
               cellFormat.setFontBold(true);
               cellFormat.setFontColor(Color.red.getRGB());
               usacreport.FrmSacReport.jBook1.setCellFormat(cellFormat);
               usacreport.FrmSacReport.jBook1.setNumber(row, col,val);
            }
         }
         catch(com.f1j.util.F1Exception ex){}
      }
      else
      {
         try {
            usacreport.FrmSacReport.jBook1.setNumber(row, col, val);
         }
          catch(com.f1j.util.F1Exception ex){}
      }
      return 0;
   }



   //从sql语句中获得表名
   private String GetSQLTableName(String SQL){
      //从SQL中获得表名
      String tablename = "";
      String word[] = SQL.split(" ");
      for (int i = 0; i < word.length; i++) {
         word[i] = word[i].trim();
      }

      for (int i = 0; i < word.length; i++) {
         if (word[i].compareToIgnoreCase("") == 0)
            continue;
         if (word[i].toLowerCase().indexOf("from") >= 0) {
            tablename = word[i + 1];
            tablename = tablename.trim();
            break;
         }
      }
      return tablename;
   }

   private String[] GetSQLFieldName(String SQL){
      String fields[]=new String[255];
      String retFields[]=null;
      int pos1=0;
      int fieldsCnt=0;

      String word[] = SQL.split(" ");
      for (int i = 0; i < word.length; i++) {
         word[i] = word[i].trim();
      }

      for (int i = 0; i < word.length; i++) {
         if (word[i].compareToIgnoreCase("") == 0) {
            continue;
         }

         if (word[i].compareToIgnoreCase("select") == 0) {
            pos1 = i;
         }
         if (word[i].compareToIgnoreCase("from") == 0) {
            //处理之前的选择字段
            for (int j = pos1 + 1; j < i; j++) {
               if (word[j].compareToIgnoreCase("") == 0 || word[j].compareToIgnoreCase(",")==0) {
                  continue;
               }
               fields[fieldsCnt]=word[j];
               fieldsCnt++;
            }
         }
      }
      if(fieldsCnt>0){
         retFields=new String[fieldsCnt];
         System.arraycopy(fields,0,retFields,0,fieldsCnt);
      }
      return retFields;
   }


   //根据表名来转换一个查询日期,返回转换好的日期
   private String ConvertDate(String SQL,Calendar date){
      String strDate="";
      String strTableName=GetSQLTableName(SQL);
      SimpleDateFormat form=new SimpleDateFormat();
      if(strTableName.indexOf("设备运行统计表")>=0){
         form.applyPattern("yyyyMMdd");
         strDate=form.format(date.getTime());
      }
      else if(strTableName.indexOf("电力历史事项表")>=0){
         form.applyPattern("yyyyMMdd");
         strDate=form.format(date.getTime());
      }
      else if(strTableName.indexOf("monthyc")>=0){
         int year=date.get(Calendar.YEAR);
         int month=date.get(Calendar.MONTH)+1;
         strDate=String.valueOf((year-1970)*12+month);
      }
      else{   //默认的转换
         strDate = String.valueOf(((int)(date.getTime().getTime() /
                                                1000) +
                                        8 * 60 * 60) / 86400);
      }
      return strDate;
   }

   private String ConvertTime(String SQL,Calendar time){
      String strTime="";
      String strTableName=GetSQLTableName(SQL);

      strTime=Integer.toString(time.get(Calendar.HOUR_OF_DAY) * 60 +
                               time.get(Calendar.MINUTE));
      int nTime=Integer.parseInt(strTime);

      if(nTime==0) //特殊处理,这里还是有问题的,因为我们的很多历史表0点是上日的1440分钟值。目前SQL语句无法表达。
      {
         strTime="1440";
      }
      return strTime;
   }




   //需要注意的是目前的开始日期，和结束日期分别是选择的日期的1日和下月的1日值。
   //除非在查询中使用指定日的方式，否则就是1日的值。
   void SQL_proc(String orgSQL, Calendar beginDate, Calendar endDate, Calendar nowDate,
                 int row, int col) {

      SimpleDateFormat form = new SimpleDateFormat("yyyy-MM-dd HH:mm");
      if(PreProcessor.SQL_DEBUG==true){
         PreProcessor.WriteLogFile("SQL_proc函数执行参数－当前时间:" +
                                   form.format(nowDate.getTime()) +
                                   "  开始时间：" + form.format(beginDate.getTime()) +
                                   "  结束时间:" + form.format(endDate.getTime()));
         PreProcessor.WriteLogFile("原始SQL代码:" + orgSQL);
      }
      Calendar begin=Calendar.getInstance();
      Calendar end=Calendar.getInstance();
      Calendar now=Calendar.getInstance();
      begin.setTime(beginDate.getTime());
      end.setTime(endDate.getTime());
      now.setTime(nowDate.getTime());



      final class FieldsFormat{
         public int field=0;                //指定显示的字段
         public String formatStr="";        //指定字段显示的格式
         public String typeStr="";      //指定该字段原始类型，数据库表相关类型
      }

      final class Arrange {
         public String direction = "v"; //v：垂直排列,h:横向排列
         public boolean autoArrange = true; //自动排列数据项目
         public int maxArrangeCnt = 0; //排列最大个数,0未设置
      }

      final class DateTimeVSValue {
         public boolean useSign = false;      //结果集是否是时间值对。
         public boolean showDateTime = false; //是否显示日期时间对的日期时间，2列或2行，
         public int DateTimeBeginVal = 0;     //日期时间的开始值
         public String defaultVal = "";       //没有时间点缺省的值。
         public int interval = 0;             //时间值对的时间间隔。0不处理间隔，直接显示
         public String unit = "";             //间隔单位y,M,d,h,m;
         public String DatetimeShowFormatStr = ""; //显示可视化串：hh:mm:ss,yyyy-MM-dd等
         public String ValueShowFormatStr = "";
      }

      //分析SQL字符串
      String pseudoSQL;
      String realSQL;
      int pos, spos, epos;
      FieldsFormat fieldsFormat[]=null;
      Arrange arrange = new Arrange();
      DateTimeVSValue timeVSVal = new DateTimeVSValue();
      String tmpStr[] = orgSQL.split(";");

      for (int i = 0; i < tmpStr.length; i++) {
         tmpStr[i] = tmpStr[i].trim();
      }
      pseudoSQL = tmpStr[0];

      for (int i = 1; i < tmpStr.length; i++) {
         if (tmpStr[i].matches(".*fieldsShowformat:.*")) {
            String strSub = tmpStr[i].substring(17, tmpStr[i].length());
            String tmpStrSub[] = strSub.split(",");
            fieldsFormat = new FieldsFormat[tmpStrSub.length];
            for(int j=0;j<tmpStrSub.length;j++){
               fieldsFormat[j]=new FieldsFormat();
            }
            for (int j = 0; j < tmpStrSub.length; j++) {
               if(tmpStrSub[j].matches(".*field\\[\\d{1,2}\\].*format\\[.*\\]")  ||
                  tmpStrSub[j].matches(".*field\\[\\d{1,2}\\].*format\\[.*\\].*type\\[.*\\]") ){
                  spos=tmpStrSub[j].indexOf("field");
                  spos=tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr,spos);
                  epos=tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr,spos);
                  fieldsFormat[j].field=Integer.parseInt(tmpStrSub[j].substring(spos + 1, epos));

                  spos=tmpStrSub[j].indexOf("format");
                  spos=tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr,spos);
                  epos=tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr,spos);
                  fieldsFormat[j].formatStr=tmpStrSub[j].substring(spos + 1, epos);

                  spos = tmpStrSub[j].indexOf("type");
                  if(spos==-1)
                     continue;
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr, spos);
                  epos = tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr, spos);
                  fieldsFormat[j].typeStr = tmpStrSub[j].substring(spos + 1, epos);
               }
            }
         }

         if (tmpStr[i].matches("arrange:.*")) {
            String strSub = tmpStr[i].substring(8, tmpStr[i].length());
            String tmpStrSub[] = strSub.split(",");
            for (int j = 0; j < tmpStrSub.length; j++) {
               if (tmpStrSub[j].matches("\\s*v\\s*")) {
                  arrange.direction = "v";
               }
               else if (tmpStrSub[j].matches("\\s*h\\s*")) {
                  arrange.direction = "h";
               }
               else if (tmpStrSub[j].matches("\\s*maxcount\\s*")) {
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr);
                  epos = tmpStrSub[j].lastIndexOf(sqlConstantDef.eSplitStr);
                  if (spos < 0 || epos < 0) {
                     break;
                  }
                  arrange.maxArrangeCnt =
                      Integer.parseInt(tmpStrSub[j].substring(spos + 1, epos));
               }
            }
            continue;
         } //end if arrange

         if (tmpStr[i].matches("datetimeVSvalue:.*")) {
            String strSub = tmpStr[i].substring(16, tmpStr[i].length());
            String tmpStrSub[] = strSub.split(",");
            for (int j = 0; j < tmpStrSub.length; j++) {
               if (tmpStrSub[j].matches("\\s*yes\\s*")) {
                  timeVSVal.useSign = true;
               }
               else if (tmpStrSub[j].matches("\\s*no\\s*")) {
                  timeVSVal.useSign = false;
                  break; //不需要在处理了
               }
               else if (tmpStrSub[j].matches("\\s*show\\s*")) {
                  timeVSVal.showDateTime = true;
               }
               else if (tmpStrSub[j].matches("\\s*hide\\s*")) {
                  timeVSVal.showDateTime = false;
               }
               else if (tmpStrSub[j].matches("\\s*datetimeBeginVal.*")) {
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr);
                  epos = tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr);
                  if (spos < 0 || epos < 0) {
                     break;
                  }
                  timeVSVal.DateTimeBeginVal =
                      Integer.parseInt(tmpStrSub[j].substring(spos + 1, epos));
               }
               else if (tmpStrSub[j].matches("\\s*defaultVal.*")) {
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr);
                  epos = tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr);
                  if (spos < 0 || epos < 0) {
                     break;
                  }
                  timeVSVal.defaultVal = tmpStrSub[j].substring(spos + 1, epos);
               }
               else if (tmpStrSub[j].matches("\\s*Interval.*")) {
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr);
                  epos = tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr);
                  if (spos < 0 || epos < 0) {
                     break;
                  }
                  String IntevalStr = tmpStrSub[j].substring(spos + 1, epos);
                  spos = IntevalStr.indexOf('m');
                  if (spos > 0) {
                     timeVSVal.interval = Integer.parseInt( (IntevalStr.
                         substring(0, spos).trim()));
                     timeVSVal.unit = "m";
                  }
                  spos = IntevalStr.indexOf('d');
                  if (spos > 0) {
                     timeVSVal.interval = Integer.parseInt( (IntevalStr.
                         substring(0, spos).trim()));
                     timeVSVal.unit = "d";
                  }
                  spos = IntevalStr.indexOf('M');
                  if (spos > 0) {
                     timeVSVal.interval = Integer.parseInt( (IntevalStr.
                         substring(0, spos).trim()));
                     timeVSVal.unit = "M";
                  }
                  spos = IntevalStr.indexOf('y');
                  if (spos > 0) {
                     timeVSVal.interval = Integer.parseInt( (IntevalStr.
                         substring(0, spos).trim()));
                     timeVSVal.unit = "y";
                  }
               }
               else if (tmpStrSub[j].matches("\\s*DatetimeShowFormat.*")) {
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr);
                  epos = tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr);
                  if (spos < 0 || epos < 0) {
                     break;
                  }
                  timeVSVal.DatetimeShowFormatStr = tmpStrSub[j].substring(spos +
                      1, epos);
               }
               else if (tmpStrSub[j].matches("\\s*ValueShowFormat.*")) {
                  spos = tmpStrSub[j].indexOf(sqlConstantDef.sSplitStr);
                  epos = tmpStrSub[j].indexOf(sqlConstantDef.eSplitStr);
                  if (spos < 0 || epos < 0) {
                     break;
                  }
                  timeVSVal.ValueShowFormatStr = tmpStrSub[j].substring(spos +
                      1, epos);
               }

            } //end for j
         } //endif datetimeVSvalue
      }

      //替换语句中的伪指令，生成能够执行的标准SQL;
      //开始和结束日期可能会在函数中被改变。
      //hongxiang 广州测试添加  处理季报
       if (usacreport.FrmSacReport.reportflag == 3) {
         g_gjd = jComJD.getSelectedIndex();
         String strtempjd = String.valueOf(now.get(now.YEAR) * 10000 +
                                 (now.get(now.MONTH) + 1) * 100 +
                                 now.get(now.DAY_OF_MONTH));

         if(g_gjd == 0){     //第一季度
            try {
               usacreport.FrmSacReport.jBook1.setActiveCell(1, 1);
            }
            catch (F1Exception ex2) {
            }
            try {
               usacreport.FrmSacReport.jBook1.setCellText("第一季度");
            }
            catch (F1Exception ex1) {
            }


            pseudoSQL =pseudoSQL + "and sdate>=[begin:" + strtempjd.substring(0, 4) + "-01-01] and sdate<[end:" + strtempjd.substring(0, 4) + "-04-01] and flag=153 order by sdate";
         }
         else if(g_gjd == 1){ //第二季度
            try {
               usacreport.FrmSacReport.jBook1.setActiveCell(1, 1);
            }
            catch (F1Exception ex2) {
            }
            try {
               usacreport.FrmSacReport.jBook1.setCellText("第二季度");
            }
            catch (F1Exception ex1) {
            }

            pseudoSQL =pseudoSQL + "and sdate>=[begin:" + strtempjd.substring(0, 4) + "-04-01] and sdate<[end:" + strtempjd.substring(0, 4) + "-07-01] and flag=153 order by sdate";
         }
         else if(g_gjd == 2){ //第三季度
            try {
               usacreport.FrmSacReport.jBook1.setActiveCell(1, 1);
            }
            catch (F1Exception ex2) {
            }
            try {
               usacreport.FrmSacReport.jBook1.setCellText("第三季度");
            }
            catch (F1Exception ex1) {
            }

            pseudoSQL =pseudoSQL + "and sdate>=[begin:" + strtempjd.substring(0, 4) + "-07-01] and sdate<[end:" + strtempjd.substring(0, 4) + "-10-01] and flag=153 order by sdate";
         }
         else if(g_gjd == 3){ //第四季度
            try {
               usacreport.FrmSacReport.jBook1.setActiveCell(1, 1);
            }
            catch (F1Exception ex2) {
            }
            try {
               usacreport.FrmSacReport.jBook1.setCellText("第四季度");
            }
            catch (F1Exception ex1) {
            }

            pseudoSQL =pseudoSQL + "and sdate>=[begin:" + strtempjd.substring(0, 4) + "-10-01] and sdate< = [end:" + strtempjd.substring(0, 4) + "-12-31] and flag=153 order by sdate";
         }
      }
      //hongxiang 广州测试添加  处理周报
      if (usacreport.FrmSacReport.reportflag == 2) {
         String strtemp;
     //    System.out.println("今天的日期: " + now.getTime());
         int day_of_week = now.get(Calendar.DAY_OF_WEEK) - 2;
         if(day_of_week==-1){
            now.add(Calendar.DATE,-6);
         }
         else
         now.add(Calendar.DATE, -day_of_week);
         strtemp = String.valueOf(now.get(now.YEAR) * 10000 +
                                  (now.get(now.MONTH) + 1) * 100 +
                                  now.get(now.DAY_OF_MONTH));
         try {
            usacreport.FrmSacReport.jBook1.setActiveCell(1, 1);
         }
         catch (F1Exception ex2) {
         }
         try {
            usacreport.FrmSacReport.jBook1.setCellText("begin:" + strtemp);
         }
         catch (F1Exception ex1) {
         }


         pseudoSQL += " and sdate>=  [begin:" + strtemp.substring(0, 4) + "-" +
             strtemp.substring(4, 6) + "-" + strtemp.substring(6, 8) +
             "] and sdate<=[end:";

     //    System.out.println("本周第一天: " + now.getTime());
         now.add(Calendar.DATE, 6);
         strtemp = String.valueOf(now.get(now.YEAR) * 10000 +
                                  (now.get(now.MONTH) + 1) * 100 +
                                  now.get(now.DAY_OF_MONTH));
         try {
            usacreport.FrmSacReport.jBook1.setActiveCell(1, 2);
         }
         catch (F1Exception ex2) {
         }
         try {
            usacreport.FrmSacReport.jBook1.setCellText("end:" + strtemp);
         }
         catch (F1Exception ex1) {
         }
         pseudoSQL += strtemp.substring(0, 4) + "-" + strtemp.substring(4, 6) +
             "-" + strtemp.substring(6, 8) + "] ";
      //   System.out.println("本周末: " + now.getTime());
         pseudoSQL += " order by sdate";
   //      System.out.println("本周末: " + pseudoSQL);

      }
      realSQL = ConvertSQL(pseudoSQL, begin, end, now);
      String strInfo;

/*  测试用代码
      strInfo = "now=" +
          String.valueOf(now.get(now.YEAR) * 10000 +
                         (now.get(now.MONTH) + 1) * 100 + now.get(now.DAY_OF_MONTH));
      strInfo += "  begin=" +
          String.valueOf(begin.get(begin.YEAR) * 10000 +
                         (begin.get(begin.MONTH) + 1) * 100 +
                         begin.get(begin.DAY_OF_MONTH));
      strInfo += "  end=" +
          String.valueOf(end.get(end.YEAR) * 10000 +
                         (end.get(end.MONTH) + 1) * 100 + end.get(end.DAY_OF_MONTH));
*/
      if(PreProcessor.SQL_DEBUG==true){
         PreProcessor.WriteLogFile("转换后的SQL:" + realSQL);
      }
      //========================================
      //开始准备执行SQL
      Vector content = new Vector();
/*
      //20090907考虑到了查询上月数据的可能性，扩大查询表范围
      Calendar tmpBegin=Calendar.getInstance();
      tmpBegin.setTime(begin.getTime());
      tmpBegin.add(Calendar.MONTH,-1);
 */
//hongxiang 广州测试   处理时报表
        if(usacreport.FrmSacReport.reportflag ==1){
           g_ghour=jComGHour.getSelectedIndex();
           realSQL=realSQL + "and  time >= "  + (g_ghour * 60+ 5) +"and time < =" + (g_ghour * 60 + 60);
        }
//end
        int colCnt = usacreport.DBEx.SqlReadRecorders(hdb_name, realSQL, begin,
          end, content);


      if (colCnt == 0) {
         try {
            usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
            if(PreProcessor.SQL_DEBUG)
               usacreport.FrmSacReport.jBook1.setCellText("@SQL,"+ realSQL);
            else
            {
               //lcg 20090821默认取不出来数据填充数值0；
               usacreport.FrmSacReport.jBook1.setNumber(0);
               //usacreport.FrmSacReport.jBook1.setCellText("");
            }
         }
         catch (com.f1j.util.F1Exception e1) {
         }
         return ;
      }

      //=====================================================================
      //开始格式化结果集
      if (timeVSVal.useSign) { //结果是时间值对
         if (colCnt != 2) {
            //结果集实际不是时间值对。
            timeVSVal.useSign = false;
         }

         //开始处理时间值对中丢失的时间点
         //        if(Integer.parseInt(content.get(0).toString())!=timeVSVal.DateTimeBeginVal){
         //           content.add(0,String.valueOf(timeVSVal.DateTimeBeginVal));
         //           content.add(1,timeVSVal.defaultVal);
         //        }
         if (timeVSVal.interval != 0 || timeVSVal.unit.compareTo("") < 0) {
            Calendar tmpCal = Calendar.getInstance(TimeZone.getDefault());
            if (timeVSVal.unit.startsWith("m")) { //分钟间隔
               int tmpVal = timeVSVal.DateTimeBeginVal;
               //hongxiang 广州测试、时报表
               if(usacreport.FrmSacReport.reportflag ==1){
                  tmpVal =g_ghour*60 + tmpVal;
               }
               //如果时间值对的开始值是0，则使用时间参数中的开始时间来作为时间值对的开始时间
               if(PreProcessor.UseSelHourTime && tmpVal==0){
                  tmpVal=now.get(Calendar.HOUR_OF_DAY);
                  tmpVal=tmpVal*60;
               }
               else if(PreProcessor.PWTEST && tmpVal==0)
               {
                  tmpVal=now.get(Calendar.HOUR_OF_DAY);
                  tmpVal=tmpVal*60;
               }
               for (int i = 0; i < content.size(); i += colCnt) {
                  if (tmpVal != Integer.parseInt(content.get(i).toString())) {
                     content.insertElementAt(String.valueOf(tmpVal), i);
                     content.insertElementAt(String.valueOf(timeVSVal.
                         defaultVal), i + 1);
                  }
                  //顺便格式化日期时间
                  String tmpVar=DBEx.ConvertTime(content.get(i),timeVSVal.DatetimeShowFormatStr,"");
                  content.setElementAt(tmpVar, i);
                  //=====
                  tmpVal %= 1440;
                  tmpVal += timeVSVal.interval;

                  if (timeVSVal.ValueShowFormatStr.length() >= 3) {
                     DecimalFormat valFormat = new DecimalFormat(timeVSVal.ValueShowFormatStr);
                     content.setElementAt(valFormat.format(Double.parseDouble(
                         content.get(i + 1).toString())), i + 1);
                  }
               }
            }
            else if (timeVSVal.unit.startsWith("d")) { //end if unit m
               int tmpVal = Integer.parseInt(content.get(0).toString());
               for (int i = 0; i < content.size(); i += colCnt) {
                  if (tmpVal != Integer.parseInt(content.get(i).toString())) {
                     content.insertElementAt(String.valueOf(tmpVal), i);
                     content.insertElementAt(String.valueOf(timeVSVal.
                         defaultVal), i + 1);
                  }
                  //顺便格式化日期
                  String tmpVar=DBEx.ConvertDate(content.get(i),timeVSVal.DatetimeShowFormatStr,"");
                  System.out.println(tmpVar);
                  content.setElementAt(tmpVar, i);
                  //===
                  tmpVal += timeVSVal.interval;

                  if (timeVSVal.ValueShowFormatStr.length() >= 3) {
                     DecimalFormat valFormat = new DecimalFormat(timeVSVal.
                         ValueShowFormatStr);
                     content.setElementAt(valFormat.format(Double.parseDouble(
                         content.get(i + 1).toString())), i + 1);
                  }

               }
            }
            else if (timeVSVal.unit.startsWith("M")) { //end if unit m
            }
            else if (timeVSVal.unit.startsWith("y")) { //end if unit m
            }
         } // end if inteval
      } //结束时间对处理
      else { //开始处理字段格式化串
         if (fieldsFormat != null) {
            for (int i = 0; i < fieldsFormat.length; i++) {
               int nField = fieldsFormat[i].field;
               String formatStr = fieldsFormat[i].formatStr;
               String typeStr = fieldsFormat[i].typeStr;
               if (nField > colCnt) {
                  continue;
               }
               String matchsStr;
               matchsStr =
                   "(y{4}|\\d{4})?(\\-)?(M{2}|\\d{1,2})?(\\-)?((d{2}|\\d{1,2}))?";
               if (formatStr.matches(matchsStr)) {
                  for (int j = 0; j < content.size(); j += colCnt) {
                     String tmpVar = DBEx.ConvertDate(content.get(j + nField),
                         formatStr,typeStr);
                     content.setElementAt(tmpVar, j + nField);
                  }
               }
               else {
                  matchsStr =".*(h{1,2}|m{1,2}|d{1,2}|k{1,2}).*";
                      //"(.*?:h{2}|H{2}|\\d{1,2}):(?:m{2}|\\d{1,2})(?::(?:s{2}|\\d{1,2}))?";
                  if (formatStr.matches(matchsStr)) {
                     for (int j = 0; j < content.size(); j += colCnt) {
                        String tmpVar = DBEx.ConvertTime(content.get(j + nField),
                            formatStr,typeStr);
                        content.setElementAt(tmpVar, j + nField);
                     }
                  }
                  else {
                     for (int j = 0; j < content.size(); j += colCnt) {
                        try {
                           DecimalFormat valFormat = new DecimalFormat(formatStr);
                           double tmpfloat = Double.parseDouble(content.get(j +
                               nField).toString());
                           content.setElementAt(valFormat.format(tmpfloat),
                                                j + nField);
                        }
                        catch (Exception ex) {

                        }
                     }
                  }
               }
            }
         }
      }//结束格式化处理
     // System.out.println("数据读取成功");
   //   System.out.println(content);
      //===========================================================================
      //开始处理数据的显示
      if (colCnt == 2 && timeVSVal.useSign && timeVSVal.showDateTime) { //时间值对，需要显示时间
         if (arrange.direction.startsWith("v")) { //数据垂直排列
            for (int j = 0; j < content.size(); j += colCnt) {
               for (int k = 0; k < colCnt; k++) {

                  String str1 = String.valueOf(content.get(j + k));
                  OutputCellText(row,col,str1);
                  col++;
               } //end for k
               col -= colCnt;
               row++;
            }
         }
      }
      else if (arrange.direction.startsWith("h")) {
         for (int j = 0; j < content.size(); j += colCnt) {
            for (int k = 0; k < colCnt; k++) {
               String str1 = String.valueOf(content.get(j + k));
               OutputCellText(row,col,str1);
               row++;
            } //end for k
            row -= colCnt;
            col++;
         } //end for j
      }
      else if (colCnt == 2 && timeVSVal.useSign && !timeVSVal.showDateTime) { //时间值对，不显示时间
         if (arrange.direction.startsWith("v")) {
            for (int j = 0; j < content.size(); j += colCnt) {
               String str1 = String.valueOf(content.get(j + 1));
               OutputCellText(row,col,str1);
               row++;
            }
         }
      }
      else if (arrange.direction.startsWith("h")) {
         for (int j = 0; j < content.size(); j += colCnt) {
            String str1 = String.valueOf(content.get(j + 1));
            OutputCellText(row,col,str1);
            col++;
         }
      }
      else { //默认排列方式处理
         for (int j = 0; j < content.size(); j += colCnt) {
            for (int k = 0; k < colCnt; k++) {
               String str1 = String.valueOf(content.get(j + k));
               OutputCellText(row, col, str1);
               col++;
            } //end for k
            col -= colCnt;
            row++;
         }
      }
      return;
      }

   void OutputCellText(int row, int col, String strText) {
      try {
         usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
         if (strText.matches("\\s*-?+\\d*\\s*")) {
            usacreport.FrmSacReport.jBook1.setNumber(Integer.parseInt(strText));
         }
         else if (strText.matches("\\s*-?+\\d*\\.\\d*\\s*")) {
            usacreport.FrmSacReport.jBook1.setNumber(Double.parseDouble(strText));
         }
         else {
            usacreport.FrmSacReport.jBook1.setCellText(strText);
         }
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }


   //暂时不使用了。
   private int ExecuteSQL(String realSQL, Calendar begin, Calendar end,
                          Calendar now, Vector content) {
      String dbname = hdb_name;
      int pos1 = 0, pos2 = 0;
      int fieldsCnt = 0;
      int returnnv = 0;

      String strTableName = "", strFields = "", strWhere = "", strOrder = "";
      String word[] = realSQL.split(" ");
      for (int i = 0; i < word.length; i++) {
         word[i] = word[i].trim();
      }

      for (int i = 0; i < word.length; i++) {
         if (word[i].compareToIgnoreCase("") == 0) {
            continue;
         }
         if (word[i].compareToIgnoreCase("select") == 0) {
            pos1 = i;
         }
         if (word[i].compareToIgnoreCase("from") == 0) {
            //处理之前的选择字段
            for (int j = pos1 + 1; j < i; j++) {
               if (word[j].compareToIgnoreCase("") == 0) {
                  continue;
               }
               strFields = strFields + "," + word[j];
               fieldsCnt++;
            }
            strFields = strFields.substring(1);
            strTableName = word[i + 1];
            pos1 = pos2 = 0;
         }
         if (word[i].compareToIgnoreCase("where") == 0) {
            pos1 = i;
            continue;
         }
         if (word[i].compareToIgnoreCase("order") == 0) {
            pos2 = i;
         }
      }
      if (pos1 != 0 && pos2 == 0) { //没有order段
         strWhere = "";
         for (int j = pos1 + 1; j < word.length; j++) {
            strWhere = strWhere + " " + word[j];
         }
      }
      content.removeAllElements();
//     returnnv = usacreport.DBEx.ReadRecorders(dbname, strTableName,
//                                              strFields,
//                                              strWhere, strOrder, content,
//                                              begin, end, false);
      return returnnv;
   }

   //替换sql中的伪指令。
   private String ConvertSQL(String pseudoSQL, Calendar beginDate,
                             Calendar endDate, Calendar nowDate) {
      String realSQL = "";
      String tmpStr = "";
      String timeSignStr = "";
      StringBuffer tmpStrBuf = new StringBuffer(pseudoSQL);
      Vector vecTmpStr;

      //缓存传入的时间参数
      Calendar begin = Calendar.getInstance();
      begin.setTime(beginDate.getTime());
      Calendar end = Calendar.getInstance();
      end.setTime(endDate.getTime());
      Calendar now=Calendar.getInstance();
      now.setTime(nowDate.getTime());

      int pos, spos = 0, epos = 0;



      while (epos <= tmpStrBuf.length()) {
         //[begin:yyyy-MM-dd] 或 [begin:yyyy-09-02]
         spos = tmpStrBuf.indexOf(sqlConstantDef.sSplitStr, spos);
         if (spos == -1) {
            break;
         }
         epos = tmpStrBuf.indexOf(sqlConstantDef.eSplitStr, spos);
         if (epos < 0) {
            break;
         }
         tmpStr = tmpStrBuf.substring(spos + 1, epos);
         pos = tmpStr.indexOf(sqlConstantDef.keySignSplitStr);
         if (pos < 0) {
            break;
         }
         //yyyy-MM-dd  yyyy-09-02
         String dtStr = tmpStr.substring(pos + 1, tmpStr.length());

         //begin,end,now,以及日期计算部分,下面代码获得在日期上可能的操作和值.
         //支持+|-n[d|M|y]的操作形式.
         int operand=0;     //日期表达式中的操作数,支持整数操作,但需要注意意义.
         int operandUnit=0; //日期表达式中的单位,类似于Calendar.YEAR定义
         String signStr="";
         String signOptStr=tmpStr.substring(0, pos);
         //sample begin+1d
         if(signOptStr.matches("(\\s*begin\\s*|\\s*now\\s*|\\s*end\\s*)[+|-]\\s*\\d+?[d|M|y]\\s*"))
         {
            int pos0;
            boolean isPlusVal=true;
            pos0=signOptStr.indexOf("+");
            if(pos0>0){
               signStr=signOptStr.substring(0,pos0);
               isPlusVal=true;
            }
            else{
               pos0=signOptStr.indexOf("-");
               if(pos0>0){
                  isPlusVal=false;
                  signStr=signOptStr.substring(0,pos0);
               }
               else{
                  return "";
               }
            }
            String indicateStr=signOptStr.substring(pos0,signOptStr.length()).trim();
            if(isPlusVal)
               operand=Integer.parseInt(indicateStr.substring(1,indicateStr.length()-1).trim());
            else
               operand=Integer.parseInt(indicateStr.substring(0,indicateStr.length()-1).trim());

            if(indicateStr.indexOf(sqlConstantDef.YearChar)>=0)
               operandUnit=Calendar.YEAR;
            else if(indicateStr.indexOf(sqlConstantDef.MonthChar)>=0)
               operandUnit=Calendar.MONTH;
            else if(indicateStr.indexOf(sqlConstantDef.DayChar)>=0)
               operandUnit=Calendar.DAY_OF_MONTH;
         }
         else if (signOptStr.matches("\\s*begin\\s*|\\s*end\\s*|\\s*now\\s*")){
            signStr=signOptStr;
            operandUnit=0;
         }
         else{
            return "";
         }

         //开始分析时间串，并替换为实际查询的时间格式
         String dateStr[] = new String[3];
         String timeStr[] = new String[3];
         String matchsStr = "";
         //(y{4}|\d{4})\-(M{2}|\d{1,2})(\-(d{2}|\d{1,2}))?
         //(h{2}|\d{1,2}):(m{2}|\d{1,2})(:(s{2}|\d{1,2}))?
         matchsStr =
             "(?:y{4}|\\d{4})\\-(?:M{2}|\\d{1,2})(?:\\-(?:d{2}|\\d{1,2}))?";
         if (dtStr.matches(matchsStr)) {
            dateStr = dtStr.split("\\-", 3);
         }
         matchsStr =
             "(?:h{2}|\\d{1,2}):(?:m{2}|\\d{1,2})(?::(?:s{2}|\\d{1,2}))?";
         if (dtStr.matches(matchsStr)) {
            timeStr = dtStr.split(":", 3);
         }
         int fixVal = 0;
         //pos=dtStr.indexOf("var");
         //替换yyyy,MM,dd和hh,mm,ss内容
         //并生成系统特定格式的时间字符表示
         if (dateStr.length > 0 && (dateStr[0]!=null && dateStr[1]!=null && dateStr[2]!=null)  ) {
            if (!dateStr[0].matches("\\s*yyyy")) {
               fixVal = Integer.parseInt(dateStr[0]);
               if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
                  begin.set(Calendar.YEAR, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
                  end.set(Calendar.YEAR, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
                  now.set(Calendar.YEAR, fixVal);
               }
            }
            if (!dateStr[1].matches("MM")) {
               fixVal = Integer.parseInt(dateStr[1]);
               if (fixVal > 0) {
                  fixVal -= 1;
               }
               else {
                  fixVal = 11;
               }
               if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
                  begin.set(Calendar.MONTH, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
                  end.set(Calendar.MONTH, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
                  now.set(Calendar.MONTH, fixVal);
               }
            }
            if (!dateStr[2].matches("dd") && dateStr[2] != "") {
               fixVal = Integer.parseInt(dateStr[2]);
               if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
                  begin.set(Calendar.DAY_OF_MONTH, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
                  end.set(Calendar.DAY_OF_MONTH, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
                  //这里处理如果指定了日期,并且指定的日期超出当月最大日期,则不处理该SQL
                  int nMaxDayOfMon=now.getActualMaximum(Calendar.DAY_OF_MONTH);
                  if(nMaxDayOfMon<fixVal){
                     return "";
                  }
                  else
                     now.set(Calendar.DAY_OF_MONTH, fixVal);
               }
            }
         }
         else if (timeStr.length > 0 && timeStr[0]!=null && timeStr[1]!=null && timeStr[2]!=null) {
            if (!timeStr[0].matches("\\s*hh")) {
               fixVal = Integer.parseInt(timeStr[0]);
               if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
                  begin.set(Calendar.HOUR_OF_DAY, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
                  end.set(Calendar.HOUR_OF_DAY, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
                  now.set(Calendar.HOUR_OF_DAY, fixVal);
               }
            }
            if (!timeStr[1].matches("mm")) {
               fixVal = Integer.parseInt(timeStr[1]);
               if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
                  begin.set(Calendar.MINUTE, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
                  end.set(Calendar.MINUTE, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
                  now.set(Calendar.MINUTE, fixVal);
               }
            }
            if (!timeStr[2].matches("ss")) {
               fixVal = Integer.parseInt(timeStr[2]);
               if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
                  begin.set(Calendar.SECOND, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
                  end.set(Calendar.SECOND, fixVal);
               }
               else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
                  now.set(Calendar.SECOND, fixVal);
               }
            }
         }
         else
            return "";


         //生成表的时间串，可能的选择是时间和日期的不同格式，目前时间是按分钟，日期是按天1970年以来的.
         //这里需要根据查询的表名来获得时间和日期的格式,默认的是1970年来的格式.
         //日期如14123,如20071109.

         //对beginDate和endDate需要做处理,如果内部计算出的begin小,则使用内部计算出的结果
         //如果内部计算出的end大,则使用内部计算出的结果,以使查询到的结果集正确,合理.
         String realDate = "", realTime = "";
         Calendar tmpCal=Calendar.getInstance();
         if (signStr.startsWith(sqlConstantDef.stimeSignStr)) {
            tmpCal.setTime(begin.getTime());
            if(operand!=0 && operandUnit!=0){
               tmpCal.add(operandUnit,operand);
            }
            realDate = ConvertDate(pseudoSQL,tmpCal);
            realTime = ConvertTime(pseudoSQL,tmpCal);

            //修改传入的参数，并带出函数。
            if (tmpCal.getTimeInMillis() < beginDate.getTimeInMillis()) {
               beginDate.setTime(tmpCal.getTime());
            }
         }
         else if (signStr.startsWith(sqlConstantDef.etimeSignStr)) {
            tmpCal.setTime(end.getTime());
            if(operand!=0 && operandUnit!=0){
               tmpCal.add(operandUnit,operand);
            }
            realDate =  ConvertDate(pseudoSQL,tmpCal);
            realTime = ConvertTime(pseudoSQL,tmpCal);
            //修改传入的参数，并带出函数。
            if(tmpCal.getTimeInMillis() > endDate.getTimeInMillis()){
               endDate.setTime(tmpCal.getTime());
            }
         }
         else if (signStr.startsWith(sqlConstantDef.ntimeSignStr)) {
            tmpCal.setTime(now.getTime());
            if(operand!=0 && operandUnit!=0){
               tmpCal.add(operandUnit,operand);
            }
            realDate =  ConvertDate(pseudoSQL,tmpCal);
            realTime = ConvertTime(pseudoSQL,tmpCal);
            //修改传入的参数，并带出函数。
            if (beginDate.getTimeInMillis() > tmpCal.getTimeInMillis()) {
               beginDate.setTime(tmpCal.getTime());
            }
            if (endDate.getTimeInMillis() < tmpCal.getTimeInMillis()) {
               endDate.setTime(tmpCal.getTime());
            }
         }
         matchsStr = "(y{4}|\\d{4})\\-(M{2}|\\d{1,2})(\\-(d{2}|\\d{1,2}))?";
         if (dtStr.matches(matchsStr)) {
            tmpStrBuf.replace(spos, epos + 1, realDate);
         }
         matchsStr = "(h{2}|\\d{1,2}):(m{2}|\\d{1,2})(:(s{2}|\\d{1,2}))?";
         if (dtStr.matches(matchsStr)) {
            tmpStrBuf.replace(spos, epos + 1, realTime);
         }
         //删除并替换原来伪字串
         epos = spos;
      }//end while
      realSQL = tmpStrBuf.toString();
      return realSQL;
   }

   int day_single(String type, String name, String tabletime, int begin,
                  int end, int nowdate, int selhour, int row, int col) {
      String dbname = hdb_name;
      String tablename = "";
      int nFlag = 0;
      if (type.compareTo("ea") == 0) {
         nFlag = 51;
      }
      else if (type.compareTo("eb") == 0) {
         nFlag = 52;
      }
      else if (type.compareTo("ec") == 0) {
         nFlag = 53;
      }
      else if (type.compareTo("e1") == 0 || type.compareTo("e2") == 0) {
         nFlag = 55;
      }
      else if (type.compareTo("e3") == 0 || type.compareTo("e4") == 0) {
         nFlag = 56;
      }
      else if (type.compareTo("e5") == 0) {
         nFlag = 57;
      }
      else if (type.compareTo("e6") == 0) {
         nFlag = 58;
      }
      else if (type.compareTo("e7") == 0) {
         nFlag = 59;
      }
      else if (type.compareTo("e8") == 0) {
         nFlag = 62;
      }
      else if (type.compareTo("e9") == 0) {
         nFlag = 63;
      }
      else if (type.compareTo("ed") == 0) {
         nFlag = 64;
      }
      else if (type.compareTo("ee") == 0) {
         nFlag = 65;
      }
      else if (type.compareTo("ef") == 0) {
         nFlag = 66;
      }
      else if (type.compareTo("eg") == 0) {
         nFlag = 67;
      }
      else if (type.compareTo("eh") == 0) {
         nFlag = 68;
      }
      else if (type.compareTo("ei") == 0) {
         nFlag = 69;
      }
      else if (type.compareTo("ej") == 0) {
         nFlag = 70;
      }
      else if (type.compareTo("ek") == 0) {
         nFlag = 71;
      }
      else if (type.compareTo("el") == 0) {
         nFlag = 72;
      }
      else if (type.compareTo("em") == 0) {
         nFlag = 73;
      }
      else if (type.compareTo("en") == 0) {
         nFlag = 74;
      }
      else if (type.compareTo("eo") == 0) {
         nFlag = 75;
      }
      else if (type.compareTo("ep") == 0) {
         nFlag = 79;
      }
      else if (type.compareTo("eq") == 0) {
         nFlag = 80;
      }
      else if (type.compareTo("er") == 0) {
         nFlag = 81;
      }
      else if (type.compareTo("es") == 0 || type.compareTo("et") == 0) {
         nFlag = 150;
      }
      else if (type.compareTo("eu") == 0 || type.compareTo("ev") == 0) {
         nFlag = 151;
      }
      else if (type.compareTo("ew") == 0) {
         nFlag = 152;
      }
      else if (type.compareTo("ex") == 0) {
         nFlag = 153;
      }
      else if (type.compareTo("ey") == 0) {
         nFlag = 154;
      }
      else if (type.compareTo("ez") == 0) {
         nFlag = 158;
      }
      else if (type.compareTo("e11") == 0) {
         nFlag = 159;
      }
      else if (type.compareTo("e12") == 0) {
         nFlag = 162;
      }
      else if (type.compareTo("e13") == 0) {
         nFlag = 163;
      }
      else if (type.compareTo("e14") == 0) {
         nFlag = 165;
      }
      else if (type.compareTo("e15") == 0) {
         nFlag = 166;
      }
      else if (type.compareTo("e16") == 0) {
         nFlag = 167;
      }
      else if (type.compareTo("e17") == 0) {
         nFlag = 168;
      }
      else if (type.compareTo("e18") == 0) {
         nFlag = 169;
      }
      else if (type.compareTo("e19") == 0) {
         nFlag = 170;
      }
      else if (type.compareTo("e20") == 0) {
         nFlag = 171;
      }
      else if (type.compareTo("e21") == 0) {
         nFlag = 172;
      }
      else if (type.compareTo("e22") == 0) {
         nFlag = 180;
      }
      else if (type.compareTo("e23") == 0) {
         nFlag = 200;
      }
      else if (type.compareTo("e24") == 0) {
         nFlag = 201;
      }
      else {
         return -1;
      }

      usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
      usac_vec.row = row;
      usac_vec.col = col;
      usac_vec.str_cell = s_cellvalue;
      veccontent.addElement(usac_vec);

      if ( nFlag == 52 || nFlag == 53 || nFlag == 79 ||
          nFlag == 70 || nFlag == 71
          || nFlag == 72 || nFlag == 73 || nFlag == 74 || nFlag == 75 ||
          nFlag == 79 || nFlag == 170
          || nFlag == 171 || nFlag == 172 || nFlag == 180) {
         if (selhour == 0) {
            tablename = "daykwh" + tabletime;
         }
         else {
            tablename = "kwh" + tabletime;
         }
      }
      else {
         if (selhour == 0) {
            tablename = "dayyc" + tabletime;
         }
         else {
            tablename = "yc" + tabletime;
         }

      }
      String fieldname = "";
      if (selhour == 0) {
         fieldname = "data,otime";
      }
      else {
         fieldname = "data";
      }

      String where = "", order = "";
      if (selhour == 0) {
//      where = "name = '" + name + "' and odate = " + Integer.toString(nowdate)
//            + " and flag = " + Integer.toString(nFlag);
         where = "name = '" + name + "' and sdate = " +
             Integer.toString(nowdate)
             + " and flag = " + Integer.toString(nFlag);
      }
      else {
         if(nFlag == 51){
            where = "name = '" + name + "' and sdate = " +
                Integer.toString(nowdate)
                + " and flag =51" + " and time = " +
                Integer.toString(selhour * 60);
         }
         else
         {
            where = "name = '" + name + "' and odate = " +
                Integer.toString(nowdate)
                + " and flag < 50" + " and otime = " +
                Integer.toString(selhour * 60);
         }
      }

//    System.out.println("日单点检索条件为:" + where);
      int[] icount = new int[1];
      int i = 0, j = 0;
      int pos = row;

      try {
         usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
      }
      catch (com.f1j.util.F1Exception e2) {
      }
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (returnv <= 0) {
         try {
            if (type.compareTo("e2") == 0 || type.compareTo("e4") == 0 ||
                type.compareTo("ef") == 0
                || type.compareTo("eg") == 0 || type.compareTo("eh") == 0 ||
                type.compareTo("ei") == 0
                || type.compareTo("eq") == 0 || type.compareTo("er") == 0 ||
                type.compareTo("et") == 0
                || type.compareTo("ev") == 0 || type.compareTo("e12") == 0 ||
                type.compareTo("e13") == 0
                || type.compareTo("e16") == 0 || type.compareTo("e17") == 0) {
               usacreport.FrmSacReport.jBook1.setCellText("00:00");
               fillcellpara(name, tablename, 1, nowdate, 0, type, row, col, 1);
            }
            else {
               usacreport.FrmSacReport.jBook1.setNumber(0.0000);
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            0);
            }
            return row;
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      if (selhour != 0) {
         double hour_rul = 0.0;
         try {
            fillcellpara(name, tablename, nFlag, nowdate, selhour, type, row,
                         col, 0);
            String str1 = new String(buffer[0]); //data
            str1 = str1.trim();
            hour_rul = Double.parseDouble(str1);
            if (usacreport.FrmSacReport.babs) {
               if (hour_rul < 0) {
                  hour_rul = Math.abs(hour_rul);
               }
            }
            usacreport.FrmSacReport.jBook1.setNumber(hour_rul);
         }
         catch (com.f1j.util.F1Exception e2) {
         }
         return pos;
      }

      if (type.compareTo("e2") == 0 || type.compareTo("e4") == 0 ||
          type.compareTo("ef") == 0
          || type.compareTo("eg") == 0 || type.compareTo("eh") == 0 ||
          type.compareTo("ei") == 0
          || type.compareTo("eq") == 0 || type.compareTo("er") == 0 ||
          type.compareTo("et") == 0
          || type.compareTo("ev") == 0 || type.compareTo("e12") == 0 ||
          type.compareTo("e13") == 0
          || type.compareTo("e16") == 0 || type.compareTo("e17") == 0) {
         int m_result = 0;
         try {
            String str1 = "";
            fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 1);
            //这里代码暂时这样改，实际上日最大，最小时间取的是data字段值
            //而对于发生时间这样的统计值，通常取的是otime字段值。
            if (type.compareTo("et") == 0 || type.compareTo("ev") == 0 ||
                type.compareTo("e2") == 0 || type.compareTo("e4") == 0) {
               str1 = new String(buffer[1]); //otime
            }
            else {
               str1 = new String(buffer[0]); //data
            }

            str1 = str1.trim();
            m_result = (int) Double.parseDouble(str1);
            //这里处理实时时间，实时时间按秒统计
            if (type.compareTo("eg") == 0 || type.compareTo("eh") == 0) {
               m_result /= 60;
            }

            int hour, min;
            hour = m_result / 60;
            min = m_result % 60;
            String str_hour = "", str_min = "";
            if (hour < 10) {
               str_hour = "0" + Integer.toString(hour);
            }
            else {
               str_hour = Integer.toString(hour);
            }
            if (min < 10) {
               str_min = "0" + Integer.toString(min);
            }
            else {
               str_min = Integer.toString(min);
            }
            usacreport.FrmSacReport.jBook1.setCellText(str_hour + ":" + str_min);
         }
         catch (com.f1j.util.F1Exception e3) {
         }
      }
      else {
         double m_result = 0.0;
         try {
            fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 0);
            String str1 = new String(buffer[0]); //data
            str1 = str1.trim();
            m_result = Double.parseDouble(str1);
            if (usacreport.FrmSacReport.babs) {
               if (m_result < 0) {
                  m_result = Math.abs(m_result);
               }
            }
            usacreport.FrmSacReport.jBook1.setNumber(m_result);
         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }
      return pos;
   }

   void jComyear_actionPerformed(ActionEvent e) {
      fill_cal();
   }

   void jComday_actionPerformed(ActionEvent e) {
      fill_cal();
   }

   void jComHour_actionPerformed(ActionEvent e) {
      fill_cal();
   }

////处理月报表
   int mon_gl(String type, String name, String tabletime, int begin, int end,
              int nowdate, int selhour, int row, int col) {
      String dbname = hdb_name;
      String tablename = "";
      int nFlag = 0;
      int bdir = 0;
      if (type.compareTo("ma") == 0) {
         nFlag = 51;
      }
      else if (type.compareTo("mb") == 0) {
         nFlag = 52;
      }
      else if (type.compareTo("mc") == 0) {
         nFlag = 53;
      }
      else if (type.compareTo("m1") == 0 || type.compareTo("e2") == 0) {
         nFlag = 55;
      }
      else if (type.compareTo("m3") == 0 || type.compareTo("e4") == 0) {
         nFlag = 56;
      }
      else if (type.compareTo("m5") == 0) {
         nFlag = 57;
      }
      else if (type.compareTo("m6") == 0) {
         nFlag = 58;
      }
      else if (type.compareTo("m7") == 0) {
         nFlag = 59;
      }
      else if (type.compareTo("m8") == 0) {
         nFlag = 62;
      }
      else if (type.compareTo("m9") == 0) {
         nFlag = 63;
      }
      else if (type.compareTo("md") == 0) {
         nFlag = 64;
      }
      else if (type.compareTo("me") == 0) {
         nFlag = 65;
      }
      else if (type.compareTo("mf") == 0) {
         nFlag = 66;
      }
      else if (type.compareTo("mg") == 0) {
         nFlag = 67;
      }
      else if (type.compareTo("mh") == 0) {
         nFlag = 68;
      }
      else if (type.compareTo("mi") == 0) {
         nFlag = 69;
      }
      else if (type.compareTo("mj") == 0) {
         nFlag = 70;
      }
      else if (type.compareTo("mk") == 0) {
         nFlag = 71;
      }
      else if (type.compareTo("ml") == 0) {
         nFlag = 72;
      }
      else if (type.compareTo("mm") == 0) {
         nFlag = 73;
      }
      else if (type.compareTo("mn") == 0) {
         nFlag = 74;
      }
      else if (type.compareTo("mo") == 0) {
         nFlag = 75;
      }
      else if (type.compareTo("mp") == 0) {
         nFlag = 79;
      }
      else if (type.compareTo("mq") == 0) {
         nFlag = 80;
      }
      else if (type.compareTo("mr") == 0) {
         nFlag = 81;
      }
      else if (type.compareTo("ms") == 0 || type.compareTo("mt") == 0) {
         nFlag = 150;
      }
      else if (type.compareTo("mu") == 0 || type.compareTo("mv") == 0) {
         nFlag = 151;
      }
      else if (type.compareTo("mw") == 0) {
         nFlag = 152;
      }
      else if (type.compareTo("mx") == 0) {
         nFlag = 153;
      }
      else if (type.compareTo("my") == 0) {
         nFlag = 154;
      }
      else if (type.compareTo("mz") == 0) {
         nFlag = 158;
      }
      else if (type.compareTo("m11") == 0) {
         nFlag = 159;
      }
      else if (type.compareTo("m12") == 0) {
         nFlag = 162;
      }
      else if (type.compareTo("m13") == 0) {
         nFlag = 163;
      }
      else if (type.compareTo("m14") == 0) {
         nFlag = 165;
      }
      else if (type.compareTo("m15") == 0) {
         nFlag = 166;
      }
      else if (type.compareTo("m16") == 0) {
         nFlag = 167;
      }
      else if (type.compareTo("m17") == 0) {
         nFlag = 168;
      }
      else if (type.compareTo("m18") == 0) {
         nFlag = 169;
      }
      else if (type.compareTo("m19") == 0) {
         nFlag = 170;
      }
      else if (type.compareTo("m20") == 0) {
         nFlag = 171;
      }
      else if (type.compareTo("m21") == 0) {
         nFlag = 172;
      }
      else if (type.compareTo("m22") == 0) {
         nFlag = 180;
      }
      else if (type.compareTo("m23") == 0) {
         nFlag = 200;
      }
      else if (type.compareTo("m24") == 0) {
         nFlag = 201;
      }
      else if(type.compareTo("mbs")==0){
            nFlag = 251;    //月固定时间表码值
      }
      else {
         return -1;
      }

      if (nFlag == 51 || nFlag == 52 || nFlag == 53 || nFlag == 79 ||
          nFlag == 70 || nFlag == 71
          || nFlag == 72 || nFlag == 73 || nFlag == 74 || nFlag == 75 ||
          nFlag == 170
          || nFlag == 171 || nFlag == 172 || nFlag == 180) {
         tablename = "daykwh" + tabletime;
      }
      else if(nFlag == 251){
          tablename = "kwh" + tabletime;
      }
      else {
         tablename = "dayyc" + tabletime;
      }
      String fieldname = "";
      fieldname = "data,sdate,otime";

      String where = "", order = "sdate";
      where = "name = '" + name + "' and sdate >= " + Integer.toString(begin) +
          " and sdate < " + Integer.toString(end)
          + " and flag = " + Integer.toString(nFlag);
       //hx
       if(nFlag == 251){
           fieldname = "data,sdate,time";
           where = "name = '" + name + "' and sdate >= " + Integer.toString(begin) +
                   " and sdate < " + Integer.toString(end)
                   + " and flag = " + Integer.toString(nFlag -199) + " and time = " + selhour*60;
           order = "sdate";
       }

       String str_temp = "";
      try {
         str_temp = usacreport.FrmSacReport.jBook1.getText(row + 1, col).trim();
         if (str_temp.indexOf(",") > 0) {
            str_temp = str_temp.substring(str_temp.indexOf(",") + 1,
                                          str_temp.length());
         }
         int dot_pos = 0;
         dot_pos = str_temp.indexOf(",");
         if (dot_pos > 0) {
            str_temp = str_temp.substring(0, dot_pos);
         }
         if (str_temp.compareTo(name) == 0) {
            bdir = 0; //纵向
         }
         else {
            bdir = 1; //横向
         }
      }
      catch (com.f1j.util.F1Exception edir) {
      }
      int[] icount = new int[1];
      int i = 0, j = 0;
      int pos = 0;
      if (bdir == 1) {
         pos = col;
         if (mon_delrowcol == true) { //删除列
            try {
               usacreport.FrmSacReport.jBook1.deleteRange(0, pos + end - begin,
                   usacreport.FrmSacReport.jBook1.getLastRow(), pos + 30,
                   usacreport.FrmSacReport.jBook1.eShiftColumns);
               mon_delrowcol = false;
            }
            catch (com.f1j.util.F1Exception edel) {}
         }
      }
      else {
         pos = row;
         if (mon_delrowcol == true) { //删除列
            try {
               usacreport.FrmSacReport.jBook1.deleteRange(pos + end - begin, 0,
                   pos + 30, usacreport.FrmSacReport.jBook1.getLastCol(),
                   usacreport.FrmSacReport.jBook1.eShiftRows);
               mon_delrowcol = false;
            }
            catch (com.f1j.util.F1Exception edel) {}
         }
      }
      if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
          type.compareTo("mf") == 0
          || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
          type.compareTo("mi") == 0
          || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
          type.compareTo("mt") == 0
          || type.compareTo("mv") == 0 || type.compareTo("m12") == 0 ||
          type.compareTo("m13") == 0
          || type.compareTo("m16") == 0 || type.compareTo("m17") == 0) {
         fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 1);
      }
      else if(type.compareTo("mbs") == 0){
          fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 0);
      }
      else {
         fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 0);
      }
      if (!isintab(tablename)) {
         try {
            if (bdir == 1) {
               for (i = col; i < col + 31; i++) {
                  if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                      type.compareTo("mf") == 0
                      || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                      type.compareTo("mi") == 0
                      || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                      type.compareTo("mt") == 0
                      || type.compareTo("mv") == 0 ||
                      type.compareTo("m12") == 0 || type.compareTo("m13") == 0
                      || type.compareTo("m16") == 0 ||
                      type.compareTo("m17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, i, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = i;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               return row;
            }
            else {
               for (i = row; i < row + 31; i++) {
                  if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                      type.compareTo("mf") == 0
                      || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                      type.compareTo("mi") == 0
                      || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                      type.compareTo("mt") == 0
                      || type.compareTo("mv") == 0 ||
                      type.compareTo("m12") == 0 || type.compareTo("m13") == 0
                      || type.compareTo("m16") == 0 ||
                      type.compareTo("m17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(i, col, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               return row;
            }
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }

      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (returnv <= 0) {
         try {
            if (bdir == 1) {
               for (i = col; i < col + 31; i++) {
                  if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                      type.compareTo("mf") == 0
                      || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                      type.compareTo("mi") == 0
                      || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                      type.compareTo("mt") == 0
                      || type.compareTo("mv") == 0 ||
                      type.compareTo("m12") == 0 || type.compareTo("m13") == 0
                      || type.compareTo("m16") == 0 ||
                      type.compareTo("m17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, i, "0-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = i;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               return row;
            }
            else {
               for (i = row; i < row + 31; i++) {
                  if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                      type.compareTo("mf") == 0
                      || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                      type.compareTo("mi") == 0
                      || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                      type.compareTo("mt") == 0
                      || type.compareTo("mv") == 0 ||
                      type.compareTo("m12") == 0 || type.compareTo("m13") == 0
                      || type.compareTo("m16") == 0 ||
                      type.compareTo("m17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(i, col, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               return row;
            }
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      int time_pos = begin, time_temp = 0;
      String str_1 = "";
      double m_result = 0.0;

      for (j = 0; j < returnv; j += 3) {
         String str1 = new String(buffer[j]); //data
         String str2 = new String(buffer[j + 1]); //odate
         String str3 = new String(buffer[j + 2]); //otime
         str1 = str1.trim(); //.substring(0,6);//锁定6位有效数字(包括小数点)
         str2 = str2.trim();
         str3 = str3.trim();
         m_result = Double.parseDouble(str1);
         if (usacreport.FrmSacReport.babs) {
            if (m_result < 0) {
               m_result = Math.abs(m_result);
            }
         }

         try {
            str_1 = Integer.toString(time_pos);
            if (Integer.parseInt(str2) < Integer.parseInt(str_1)) {
               continue; //如果有重复数据
            }
            while (str_1.compareTo(str2) != 0) {
               if (bdir == 1) {
                  usacreport.FrmSacReport.jBook1.setActiveCell(row, pos);
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = pos;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               else {
                  usacreport.FrmSacReport.jBook1.setActiveCell(pos, col);
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = pos;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                   type.compareTo("mf") == 0
                   || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                   type.compareTo("mi") == 0
                   || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                   type.compareTo("mt") == 0
                   || type.compareTo("mv") == 0 || type.compareTo("m12") == 0 ||
                   type.compareTo("m13") == 0
                   || type.compareTo("m16") == 0 || type.compareTo("m17") == 0) {
                  if (bdir == 1) {
                     usacreport.FrmSacReport.jBook1.setText(row, pos,
                         "00-00:00");
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = pos;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setText(pos, col,
                         "00-00:00");
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = col;
                     usac_vec.col = pos;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
               }
               else {
                  if (bdir == 1) {
                     usacreport.FrmSacReport.jBook1.setNumber(row, pos, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = pos;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(pos, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = pos;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(usac_vec);
                  }
               }
               pos++;
               time_pos += 1;
               str_1 = Integer.toString(time_pos);
            }
            if (bdir == 1) {
               usacreport.FrmSacReport.jBook1.setActiveCell(row, pos);
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               usac_vec.row = row;
               usac_vec.col = pos;
               usac_vec.str_cell = s_cellvalue;
               veccontent.addElement(usac_vec);
            }
            else {
               usacreport.FrmSacReport.jBook1.setActiveCell(pos, col);
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               usac_vec.row = pos;
               usac_vec.col = col;
               usac_vec.str_cell = s_cellvalue;
               veccontent.addElement(usac_vec);
            }
            if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                type.compareTo("mf") == 0
                || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                type.compareTo("mi") == 0
                || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                type.compareTo("mt") == 0
                || type.compareTo("mv") == 0 || type.compareTo("m12") == 0 ||
                type.compareTo("m13") == 0
                || type.compareTo("m16") == 0 || type.compareTo("m17") == 0) {
               Calendar caltemp1 = Calendar.getInstance(TimeZone.getDefault());
               caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
               Date dttemp = new Date();
               long ltemp = (long) Double.parseDouble(str2);
               ltemp = ltemp * (long) 86400000.0;
               dttemp.setTime(ltemp);
               caltemp1.setTime(dttemp);
               int m_temp = (int) Double.parseDouble(str3);
               int hour, min;
               hour = m_temp / 60;
               min = m_temp % 60;
               String str_hour = "", str_min = "", str_day = "";
               if (hour < 10) {
                  str_hour = "0" + Integer.toString(hour);
               }
               else {
                  str_hour = Integer.toString(hour);
               }
               if (min < 10) {
                  str_min = "0" + Integer.toString(min);
               }
               else {
                  str_min = Integer.toString(min);
               }

               if (caltemp1.get(caltemp1.DAY_OF_MONTH) > 9) {
                  str_day = Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
               }
               else {
                  str_day = "0" +
                      Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
               }
               if (bdir == 1) {
                  usacreport.FrmSacReport.jBook1.setText(row, pos,
                      str_day + "-" + str_hour + ":" + str_min);
               }
               else {
                  usacreport.FrmSacReport.jBook1.setText(pos, col,
                      str_day + "-" + str_hour + ":" + str_min);
               }
            }
            else {
               if (bdir == 1) {
                  usacreport.FrmSacReport.jBook1.setNumber(row, pos, m_result);
               }
               else {
                  usacreport.FrmSacReport.jBook1.setNumber(pos, col, m_result);
               }
//          usacreport.FrmSacReport.jBook1.setNumber(m_result);
            }
            pos++;
            time_pos += 1;
         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }
      try {
         if (bdir == 1) {
            if (pos < col + end - begin) {
               for (i = pos; i < col + end - begin; i++) {
                  if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                      type.compareTo("mf") == 0
                      || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                      type.compareTo("mi") == 0
                      || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                      type.compareTo("mt") == 0
                      || type.compareTo("mv") == 0 ||
                      type.compareTo("m12") == 0 || type.compareTo("m13") == 0
                      || type.compareTo("m16") == 0 ||
                      type.compareTo("m17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, i, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = i;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
         }
         else {
            if (pos < row + end - begin) {
               for (i = pos; i < row + end - begin; i++) {
                  if (type.compareTo("m2") == 0 || type.compareTo("m4") == 0 ||
                      type.compareTo("mf") == 0
                      || type.compareTo("mg") == 0 || type.compareTo("mh") == 0 ||
                      type.compareTo("mi") == 0
                      || type.compareTo("mq") == 0 || type.compareTo("mr") == 0 ||
                      type.compareTo("mt") == 0
                      || type.compareTo("mv") == 0 ||
                      type.compareTo("m12") == 0 || type.compareTo("m13") == 0
                      || type.compareTo("m16") == 0 ||
                      type.compareTo("m17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(i, col, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
         }
      }
      catch (com.f1j.util.F1Exception eii) {
      }
      return pos - 1;
   }

   //这里用来处理日开关和通道单点数据
   int day_K_single(String type, String name, String tabletime, int begin,
                    int end, int nowdate, int selhour, int row, int col) {

      return row;
   }

   //这里用来处理月开关和通道的单点数据
   int mon_K_single(String type, String name, String tabletime, int begin,
                    int end, int nowdate, int selhour, int row, int col) {
      String dbname = ConstantDef.MODELDB;
      String tablename = "";
      int[] icount = new int[1];
      String fieldname = "";
      String where = "", order = "";
      String tmpstr;
      Vector vecRuntime = new Vector();
      Vector vecStopcount = new Vector();
      Vector vecSGstopCount = new Vector();
      Vector vecChRuntime = new Vector();
      Vector vecChStopcount = new Vector();

      int mkz = 0;
      int mkx = 0;
      int mks = 0;
      int mty = 0;
      int mts = 0;

      if (type.compareTo("mkz") == 0 || type.compareTo("mkx") == 0 ||
          type.compareTo("mks") == 0) {
         fieldname = "dayruntime,daystopcount,sgstopcount";
         tablename = "开关变位统计表" + tabletime;
         where = "name = '" + name + "' and sdate>= " + String.valueOf(begin) +
             " and sdate<=" + String.valueOf(end);
         int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
             getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             order.getBytes(), icount, false);
         byte[][] buffer = new byte[0][0];
         if (returnv > 0) {
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            int ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            for (int i = 0; i < returnv; i += icount[0]) {
               tmpstr = new String(buffer[i]);
               tmpstr = tmpstr.trim();
               vecRuntime.addElement(tmpstr);
               tmpstr = new String(buffer[i + 1]);
               tmpstr = tmpstr.trim();
               vecStopcount.addElement(tmpstr);
               tmpstr = new String(buffer[i + 2]);
               tmpstr = tmpstr.trim();
               vecSGstopCount.addElement(tmpstr);
            }
            for (int i = 0; i < vecRuntime.size(); i++) {
               mkz += Integer.parseInt(vecRuntime.get(i).toString());
            }
            for (int i = 0; i < vecStopcount.size(); i++) {
               mkx += Integer.parseInt(vecStopcount.get(i).toString());
            }
            for (int i = 0; i < vecSGstopCount.size(); i++) {
               mks += Integer.parseInt(vecSGstopCount.get(i).toString());
            }
         }
         try {
            usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
            if (type.compareTo("mkz") == 0) {
               int minTotal = mkz / 60;
               int days = minTotal / (24 * 60);
               int hours = (minTotal % (24 * 60)) / 60;
               int minutes = minTotal - days * 24 * 60 - hours * 60;
               tmpstr = String.valueOf(days) + "天" + String.valueOf(hours) +
                   "小时"
                   + String.valueOf(minutes) + "分钟";
               usacreport.FrmSacReport.jBook1.setText(tmpstr);
            }
            else if (type.compareTo("mkx") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(mkx));
            }
            else if (type.compareTo("mks") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(mks));
            }
         }
         catch (com.f1j.util.F1Exception e) {

         }
      }
      else if (type.compareTo("mty") == 0 || type.compareTo("mts") == 0) {
         fieldname = "运行时间,统计值1";
         tablename = "设备运行统计表";
         where = "代码 = '" + name + "' and 存盘日期 like '" + tabletime + "%'";
         int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
             getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             order.getBytes(), icount, false);
         byte[][] buffer = new byte[0][0];
         if (returnv > 0) {
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            int ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            for (int i = 0; i < returnv; i += icount[0]) {
               tmpstr = new String(buffer[i]);
               tmpstr = tmpstr.trim();
               vecChRuntime.addElement(tmpstr);
               tmpstr = new String(buffer[i + 1]);
               tmpstr = tmpstr.trim();
               vecChStopcount.addElement(tmpstr);
            }
            for (int i = 0; i < vecChRuntime.size(); i++) {
               mty += Integer.parseInt(vecChRuntime.get(i).toString());
            }
            for (int i = 0; i < vecChStopcount.size(); i++) {
               mts += Integer.parseInt(vecChStopcount.get(i).toString());
            }
         }
         try {
            usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
            if (type.compareTo("mty") == 0) {
               int minTotal = mty / 60;
               int days = minTotal / (24 * 60);
               int hours = (minTotal % (24 * 60)) / 60;
               int minutes = minTotal - days * 24 * 60 - hours * 60;
               tmpstr = String.valueOf(days) + "天" + String.valueOf(hours) +
                   "小时"
                   + String.valueOf(minutes) + "分钟";
               usacreport.FrmSacReport.jBook1.setText(tmpstr);
            }
            else if (type.compareTo("mts") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(mts));
            }
         }
         catch (com.f1j.util.F1Exception e) {

         }

      }
      return row;
   }

   int mon_single(String type, String name, String tabletime, int begin,
                  int end, int nowdate, int selhour, int row, int col) {
      String dbname = ConstantDef.MODELDB;
      String tablename = "";
      int nFlag = 0;

      usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
      usac_vec.row = row;
      usac_vec.col = col;
      usac_vec.str_cell = s_cellvalue;
      veccontent.addElement(usac_vec);

      if (type.compareTo("na") == 0) {
         nFlag = 51;
      }
      else if (type.compareTo("nb") == 0) {
         nFlag = 52;
      }
      else if (type.compareTo("nc") == 0) {
         nFlag = 53;
      }
      else if (type.compareTo("n1") == 0 || type.compareTo("n2") == 0) {
         nFlag = 55;
      }
      else if (type.compareTo("n3") == 0 || type.compareTo("n4") == 0) {
         nFlag = 56;
      }
      else if (type.compareTo("n5") == 0) {
         nFlag = 57;
      }
      else if (type.compareTo("n6") == 0) {
         nFlag = 58;
      }
      else if (type.compareTo("n7") == 0) {
         nFlag = 59;
      }
      else if (type.compareTo("n8") == 0) {
         nFlag = 62;
      }
      else if (type.compareTo("n9") == 0) {
         nFlag = 63;
      }
      else if (type.compareTo("nd") == 0) {
         nFlag = 64;
      }
      else if (type.compareTo("ne") == 0) {
         nFlag = 65;
      }
      else if (type.compareTo("nf") == 0) {
         nFlag = 66;
      }
      else if (type.compareTo("ng") == 0) {
         nFlag = 67;
      }
      else if (type.compareTo("nh") == 0) {
         nFlag = 68;
      }
      else if (type.compareTo("ni") == 0) {
         nFlag = 69;
      }
      else if (type.compareTo("nj") == 0) {
         nFlag = 70;
      }
      else if (type.compareTo("nk") == 0) {
         nFlag = 71;
      }
      else if (type.compareTo("nl") == 0) {
         nFlag = 72;
      }
      else if (type.compareTo("nm") == 0) {
         nFlag = 73;
      }
      else if (type.compareTo("nn") == 0) {
         nFlag = 74;
      }
      else if (type.compareTo("no") == 0) {
         nFlag = 75;
      }
      else if (type.compareTo("np") == 0) {
         nFlag = 79;
      }
      else if (type.compareTo("nq") == 0) {
         nFlag = 80;
      }
      else if (type.compareTo("nr") == 0) {
         nFlag = 81;
      }
      else if (type.compareTo("ns") == 0 || type.compareTo("nt") == 0) {
         nFlag = 150;
      }
      else if (type.compareTo("nu") == 0 || type.compareTo("nv") == 0) {
         nFlag = 151;
      }
      else if (type.compareTo("nw") == 0) {
         nFlag = 152;
      }
      else if (type.compareTo("nx") == 0) {
         nFlag = 153;
      }
      else if (type.compareTo("ny") == 0) {
         nFlag = 154;
      }
      else if (type.compareTo("nz") == 0) {
         nFlag = 158;
      }
      else if (type.compareTo("n11") == 0) {
         nFlag = 159;
      }
      else if (type.compareTo("n12") == 0) {
         nFlag = 162;
      }
      else if (type.compareTo("n13") == 0) {
         nFlag = 163;
      }
      else if (type.compareTo("n14") == 0) {
         nFlag = 165;
      }
      else if (type.compareTo("n15") == 0) {
         nFlag = 166;
      }
      else if (type.compareTo("n16") == 0) {
         nFlag = 167;
      }
      else if (type.compareTo("n17") == 0) {
         nFlag = 168;
      }
      else if (type.compareTo("n18") == 0) {
         nFlag = 169;
      }
      else if (type.compareTo("n19") == 0) {
         nFlag = 170;
      }
      else if (type.compareTo("n20") == 0) {
         nFlag = 171;
      }
      else if (type.compareTo("n21") == 0) {
         nFlag = 172;
      }
      else if (type.compareTo("n22") == 0) {
         nFlag = 180;
      }
      else if (type.compareTo("n23") == 0) {
         nFlag = 200;
      }
      else if (type.compareTo("n24") == 0) {
         nFlag = 201;
      } //else{return -1;}

      //处理旬的数据，自定义处理模式
      if (type.substring(0, 1).compareTo("x") == 0) {
         int[] icount = new int[1];
         String fieldname = "";
         String where = "", order = "";
         if (cal_dt.get(cal_dt.MONTH) < 9) { /////表的日期
            tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)) + "0" +
                Integer.toString(cal_dt.get(cal_dt.MONTH) + 1);
         }
         else {
            tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)) +
                Integer.toString(cal_dt.get(cal_dt.MONTH) + 1);
         }

         fieldname = "data,odate,otime,flag";
         tablename = "dayyc" + tabletime;
         where = "name = '" + name + "' and sdate >= " + Integer.toString(begin) +
             " and sdate < " + Integer.toString(end)
             + " and flag in (150,151)";
         order = "odate,flag";
         int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
             getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             order.getBytes(), icount, true);
         byte[][] buffer = new byte[0][0];

         float sxmax = 0, sxmin = -1, zxmax = 0, zxmin = -1, xxmax = 0,
             xxmin = -1;
         float tmpVal;
         int tmpDate, tmpTime, tmpFlag;
         String tempStr;
         if (returnv > 0) {
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            for (int j = 0; j < returnv; j += 4) {
               tempStr = new String(buffer[j]);
               tempStr = tempStr.trim();
               tmpVal = Float.parseFloat(tempStr);
               tempStr = new String(buffer[j + 1]);
               tempStr = tempStr.trim();
               tmpDate = Integer.parseInt(tempStr);
               tempStr = new String(buffer[j + 2]);
               tempStr = tempStr.trim();
               tmpTime = Integer.parseInt(tempStr);
               tempStr = new String(buffer[j + 3]);
               tempStr = tempStr.trim();
               tmpFlag = Integer.parseInt(tempStr);
               if (tmpDate - begin <= 10) {
                  if (tmpFlag == 150 && tmpVal > sxmax) {
                     sxmax = tmpVal;
                     continue;
                  }
                  else if ( (tmpFlag == 151 && tmpVal < sxmin) ||
                           (tmpFlag == 151 && sxmin == -1)) {
                     sxmin = tmpVal;
                     continue;
                  }
               }
               else if (tmpDate - begin > 10 && tmpDate - begin <= 20) {
                  if (tmpFlag == 150 && tmpVal > zxmax) {
                     zxmax = tmpVal;
                     continue;
                  }
                  else if ( (tmpFlag == 151 && tmpVal < zxmin) ||
                           (tmpFlag == 151 && zxmin == -1)) {
                     zxmin = tmpVal;
                     continue;
                  }
               }
               else if (tmpDate - begin > 20) {
                  if (tmpFlag == 150 && tmpVal > xxmax) {
                     xxmax = tmpVal;
                     continue;
                  }
                  else if ( (tmpFlag == 151 && tmpVal < xxmin) ||
                           (tmpFlag == 151 && xxmin == -1)) {
                     xxmin = tmpVal;
                     continue;
                  }
               }
            }
         }
         if (sxmin < 0) {
            sxmin = 0;
         }
         if (zxmin < 0) {
            zxmin = 0;
         }
         if (xxmin < 0) {
            xxmin = 0;
         }
         try {
            usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
            if (type.compareTo("xsmax") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(sxmax));
            }
            else if (type.compareTo("xsmin") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(sxmin));
            }
            else if (type.compareTo("xzmax") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(zxmax));
            }
            else if (type.compareTo("xzmin") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(zxmin));
            }
            else if (type.compareTo("xxmax") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(xxmax));
            }
            else if (type.compareTo("xxmin") == 0) {
               usacreport.FrmSacReport.jBook1.setText(String.valueOf(xxmin));
            }
         }
         catch (com.f1j.util.F1Exception e) {
         }
         return row;
      }

      //处理月开关变位数据,这里需要自定义处理
      if (type.substring(0, 1).compareTo("x") == 0) {

      }

      if (nFlag == 51 || nFlag == 52 || nFlag == 53 || nFlag == 79 ||
          nFlag == 70 || nFlag == 71
          || nFlag == 72 || nFlag == 73 || nFlag == 74 || nFlag == 75 ||
          nFlag == 170
          || nFlag == 171 || nFlag == 172 || nFlag == 180) {
         tablename = "monthkwh" + tabletime;
      }
      else {
         tablename = "monthyc" + tabletime;
      }
      String fieldname = "";
      fieldname = "data,odate,otime";

      String where = "", order = "";
      if (selhour == 0) {
         where = "name = '" + name + "' and odate >= " + Integer.toString(begin) +
             " and odate < " + Integer.toString(end)
             + " and flag = " + Integer.toString(nFlag);
      }
      else {
         where = "name = '" + name + "' and odate = " +
             Integer.toString(begin - 1)
             + " and flag = " + Integer.toString(nFlag);
      }

      //System.out.println("月单点数据检索条件为:" + where);
      //System.out.println("表名为"+tablename);
      int[] icount = new int[1];
      int i = 0, j = 0;

      try {
         usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
      }
      catch (com.f1j.util.F1Exception cellact) {}
      if (!isintab(tablename)) {
         try {
            if (type.compareTo("n2") == 0 || type.compareTo("n4") == 0 ||
                type.compareTo("nf") == 0
                || type.compareTo("ng") == 0 || type.compareTo("nh") == 0 ||
                type.compareTo("ni") == 0
                || type.compareTo("nq") == 0 || type.compareTo("nr") == 0 ||
                type.compareTo("nt") == 0
                || type.compareTo("nv") == 0 || type.compareTo("n12") == 0 ||
                type.compareTo("n13") == 0
                || type.compareTo("n16") == 0 || type.compareTo("n17") == 0) {
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            1);
               usacreport.FrmSacReport.jBook1.setCellText("00-00:00");
            }
            else {
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            0);
               usacreport.FrmSacReport.jBook1.setNumber(0.0000);
            }
         }
         catch (com.f1j.util.F1Exception e) {
         }
         //2008127修改
         //return row;
      }
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (returnv <= 0) {
         try {
            if (type.compareTo("n2") == 0 || type.compareTo("n4") == 0 ||
                type.compareTo("nf") == 0
                || type.compareTo("ng") == 0 || type.compareTo("nh") == 0 ||
                type.compareTo("ni") == 0
                || type.compareTo("nq") == 0 || type.compareTo("nr") == 0 ||
                type.compareTo("nt") == 0
                || type.compareTo("nv") == 0 || type.compareTo("n12") == 0 ||
                type.compareTo("n13") == 0
                || type.compareTo("n16") == 0 || type.compareTo("n17") == 0) {
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            1);
               usacreport.FrmSacReport.jBook1.setCellText("00-00:00");
            }
            else {
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            0);
               usacreport.FrmSacReport.jBook1.setNumber(0.0000);
            }
            return row;
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      double m_result = 0.0;

      String str1 = new String(buffer[0]); //data
      str1 = str1.trim();
      String str2 = new String(buffer[1]); //odate
      str2 = str2.trim();
      String str3 = new String(buffer[2]); //otime
      str3 = str3.trim();
      if (type.compareTo("n2") == 0 || type.compareTo("n4") == 0 ||
          type.compareTo("nf") == 0
          ||
          type.compareTo("ni") == 0
          || type.compareTo("nq") == 0 || type.compareTo("nr") == 0 ||
          type.compareTo("nt") == 0
          || type.compareTo("nv") == 0
          || type.compareTo("n16") == 0 || type.compareTo("n17") == 0) {
         try {
            fillcellpara(name, tablename, nFlag, Integer.parseInt(str2), 0,
                         type, row, col, 1);
            int m_result1 = (int) Double.parseDouble(str3);
            int hour, min;
            Calendar caltemp1 = Calendar.getInstance(TimeZone.getDefault());
            caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
            Date dttemp = new Date();
            long ltemp = (long) Double.parseDouble(str2);
            ltemp = ltemp * (long) 86400000.0;
            dttemp.setTime(ltemp);
            caltemp1.setTime(dttemp);
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.YEAR))+ "年");
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.MONTH)+1));
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH)));
            hour = m_result1 / 60;
            min = m_result1 % 60;
            String str_hour = "", str_min = "", str_day = "";
            if (hour < 10) {
               str_hour = "0" + Integer.toString(hour);
            }
            else {
               str_hour = Integer.toString(hour);
            }
            if (min < 10) {
               str_min = "0" + Integer.toString(min);
            }
            else {
               str_min = Integer.toString(min);
            }
            if (caltemp1.get(caltemp1.DAY_OF_MONTH) > 9) {
               str_day = Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
            }
            else {
               str_day = "0" +
                   Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
            }
            usacreport.FrmSacReport.jBook1.setCellText(str_day + "-" + str_hour +
                ":" + str_min);
         }
         catch (com.f1j.util.F1Exception e3) {
         }
      }
      else if(type.compareTo("n12") == 0 ||
              type.compareTo("n13") == 0 ||
              type.compareTo("ng") == 0 ||
              type.compareTo("nh") == 0 ) {
         try {
            fillcellpara(name, tablename, nFlag, Integer.parseInt(str2), 0,
                         type, row, col, 1);
            int m_result1 = (int) Double.parseDouble(str1);
            int day,hour, min;
            Calendar caltemp1 = Calendar.getInstance(TimeZone.getDefault());
            caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
            Date dttemp = new Date();
            long ltemp = (long) Double.parseDouble(str2);
            ltemp = ltemp * (long) 86400000.0;
            dttemp.setTime(ltemp);
            caltemp1.setTime(dttemp);
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.YEAR))+ "年");
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.MONTH)+1));
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH)));
            min =(int) m_result1/60;  //获得总分钟,忽略秒
            day  = min / 1440;         //获得天数
            hour= (min-day*1440) /60;
            min=min %(int) 60;

            String str_hour = "", str_min = "", str_day = "";
            if(day<10)
               str_day="0"+Integer.toString(day);
            if (hour < 10) {
               str_hour = "0" + Integer.toString(hour);
            }
            else {
               str_hour = Integer.toString(hour);
            }
            if (min < 10) {
               str_min = "0" + Integer.toString(min);
            }
            else {
               str_min = Integer.toString(min);
            }
/*
            if (caltemp1.get(caltemp1.DAY_OF_MONTH) > 9) {
               str_day = Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
            }
            else {
               str_day = "0" +
                   Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
            }

            str_day=Integer.toString(day);
 */
            usacreport.FrmSacReport.jBook1.setCellText(str_day + "-" + str_hour +
                ":" + str_min);

         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }
      else {
         try {
            fillcellpara(name, tablename, nFlag, Integer.parseInt(str2), 0,
                         type, row, col, 0);

            m_result = Double.parseDouble(str1);
            if (usacreport.FrmSacReport.babs) {
               if (m_result < 0) {
                  m_result = Math.abs(m_result);
               }
            }
            usacreport.FrmSacReport.jBook1.setNumber(m_result);
         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }
      return row;
   }

////处理年报表
   int year_gl(String type, String name, String tabletime, int begin, int end,
               int nowdate, int selhour, int row, int col) {
      /////////////年的时间问题还没有处理
      String dbname = hdb_name;
      String tablename = "";
      int nFlag = 0;
      int bdir = 0;
      if (type.compareTo("ya") == 0) {
         nFlag = 51;
      }
      else if (type.compareTo("yb") == 0) {
         nFlag = 52;
      }
      else if (type.compareTo("yc") == 0) {
         nFlag = 53;
      }
      else if (type.compareTo("y1") == 0 || type.compareTo("y2") == 0) {
         nFlag = 55;
      }
      else if (type.compareTo("y3") == 0 || type.compareTo("y4") == 0) {
         nFlag = 56;
      }
      else if (type.compareTo("y5") == 0) {
         nFlag = 57;
      }
      else if (type.compareTo("y6") == 0) {
         nFlag = 58;
      }
      else if (type.compareTo("y7") == 0) {
         nFlag = 59;
      }
      else if (type.compareTo("y8") == 0) {
         nFlag = 62;
      }
      else if (type.compareTo("y9") == 0) {
         nFlag = 63;
      }
      else if (type.compareTo("yd") == 0) {
         nFlag = 64;
      }
      else if (type.compareTo("ye") == 0) {
         nFlag = 65;
      }
      else if (type.compareTo("yf") == 0) {
         nFlag = 66;
      }
      else if (type.compareTo("yg") == 0) {
         nFlag = 67;
      }
      else if (type.compareTo("yh") == 0) {
         nFlag = 68;
      }
      else if (type.compareTo("yi") == 0) {
         nFlag = 69;
      }
      else if (type.compareTo("yj") == 0) {
         nFlag = 70;
      }
      else if (type.compareTo("yk") == 0) {
         nFlag = 71;
      }
      else if (type.compareTo("yl") == 0) {
         nFlag = 72;
      }
      else if (type.compareTo("ym") == 0) {
         nFlag = 73;
      }
      else if (type.compareTo("yn") == 0) {
         nFlag = 74;
      }
      else if (type.compareTo("yo") == 0) {
         nFlag = 75;
      }
      else if (type.compareTo("yp") == 0) {
         nFlag = 79;
      }
      else if (type.compareTo("yq") == 0) {
         nFlag = 80;
      }
      else if (type.compareTo("yr") == 0) {
         nFlag = 81;
      }
      else if (type.compareTo("ys") == 0 || type.compareTo("yt") == 0) {
         nFlag = 150;
      }
      else if (type.compareTo("yu") == 0 || type.compareTo("yv") == 0) {
         nFlag = 151;
      }
      else if (type.compareTo("yw") == 0) {
         nFlag = 152;
      }
      else if (type.compareTo("yx") == 0) {
         nFlag = 153;
      }
      else if (type.compareTo("yy") == 0) {
         nFlag = 154;
      }
      else if (type.compareTo("yz") == 0) {
         nFlag = 158;
      }
      else if (type.compareTo("y11") == 0) {
         nFlag = 159;
      }
      else if (type.compareTo("y12") == 0) {
         nFlag = 162;
      }
      else if (type.compareTo("y13") == 0) {
         nFlag = 163;
      }
      else if (type.compareTo("y14") == 0) {
         nFlag = 165;
      }
      else if (type.compareTo("y15") == 0) {
         nFlag = 166;
      }
      else if (type.compareTo("y16") == 0) {
         nFlag = 167;
      }
      else if (type.compareTo("y17") == 0) {
         nFlag = 168;
      }
      else if (type.compareTo("y18") == 0) {
         nFlag = 169;
      }
      else if (type.compareTo("y19") == 0) {
         nFlag = 170;
      }
      else if (type.compareTo("y20") == 0) {
         nFlag = 171;
      }
      else if (type.compareTo("y21") == 0) {
         nFlag = 172;
      }
      else if (type.compareTo("y22") == 0) {
         nFlag = 180;
      }
      else if (type.compareTo("y23") == 0) {
         nFlag = 200;
      }
      else if (type.compareTo("y24") == 0) {
         nFlag = 201;
      }
      else {
         return -1;
      }

      if (nFlag == 51 || nFlag == 52 || nFlag == 53 || nFlag == 79 ||
          nFlag == 70 || nFlag == 71
          || nFlag == 72 || nFlag == 73 || nFlag == 74 || nFlag == 75 ||
          nFlag == 170
          || nFlag == 171 || nFlag == 172 || nFlag == 180) {
         tablename = "monthkwh" + tabletime;
      }
      else {
         tablename = "monthyc" + tabletime;
      }
      String fieldname = "";
      fieldname = "data,sdate,odate,otime";
      String where = "", order = "sdate";

      where = "name = '" + name + "' and sdate >= " + Integer.toString(begin) +
          " and sdate < " + Integer.toString(end)
          + " and flag = " + Integer.toString(nFlag);
      //System.out.println(where);
      String str_temp = "";
      try {
         str_temp = usacreport.FrmSacReport.jBook1.getText(row, col + 1).trim();
         if (str_temp.indexOf(",") > 0) {
            str_temp = str_temp.substring(str_temp.indexOf(",") + 1,
                                          str_temp.length());
         }
         int dot_pos = 0;
         dot_pos = str_temp.indexOf(",");
         if (dot_pos > 0) {
            str_temp = str_temp.substring(0, dot_pos);
         }
         if (str_temp.compareTo(name) == 0) {
            bdir = 1; //横向
         }
         else {
            bdir = 0; //纵向
         }
      }
      catch (com.f1j.util.F1Exception edir) {
      }
//    System.out.println("年规律数据检索条件为:" + where);
      int[] icount = new int[1];
      int i = 0, j = 0;
      int pos = 0;
      if (bdir == 1) {
         pos = col;
      }
      else {
         pos = row;
      }

      if (!isintab(tablename)) {
         try {
            if (bdir == 1) {
               for (i = col; i < col + 12; i++) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, i, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = i;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               return row;
            }
            else {
               for (i = row; i < row + 12; i++) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0 ||
                      type.compareTo("y17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(i, col, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               return row;
            }
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }

      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
          type.compareTo("yf") == 0
          || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
          type.compareTo("yi") == 0
          || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
          type.compareTo("yt") == 0
          || type.compareTo("yv") == 0 || type.compareTo("y12") == 0 ||
          type.compareTo("y13") == 0
          || type.compareTo("y16") == 0 || type.compareTo("y17") == 0) {
         fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 1);
      }
      else {
         fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 0);
      }
      if (returnv <= 0) {
         try {
            if (bdir == 1) {
               for (i = col; i < col + 12; i++) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, i, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = i;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
            else {
               for (i = row; i < row + 12; i++) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0 ||
                      type.compareTo("y17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(i, col, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
            return row;
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      int time_pos = begin, time_temp = 0;
      String str_1 = "";
      double m_result = 0.0;

      for (j = 0; j < returnv; j += 4) {
         String str1 = new String(buffer[j]); //data
         String str2 = new String(buffer[j + 1]); //sdate
         String str3 = new String(buffer[j + 2]); //odate
         // String str4 = new String(buffer[j+3]);//otime
         str1 = str1.trim(); //.substring(0,6);//锁定6位有效数字
         str2 = str2.trim();
         str3 = str3.trim();
         // str4 = str4.trim();
         // System.out.println(str1);
         // System.out.println(str2);
         //  System.out.println(str3);
         m_result = Double.parseDouble(str1);

         if (usacreport.FrmSacReport.babs) {
            if (m_result < 0) {
               m_result = Math.abs(m_result);
            }
         }
         try {
            str_1 = Integer.toString(time_pos);
            if (Integer.parseInt(str2) < Integer.parseInt(str_1)) {
               continue; //如果有重复数据
            }
            while (str_1.compareTo(str2) != 0) {
               if (bdir == 1) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0 ||
                      type.compareTo("y17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, pos,
                         "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, pos, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = pos;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               else {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0 ||
                      type.compareTo("y17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(pos, col,
                         "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(pos, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = pos;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
               pos++;
               time_pos += 1;
               str_1 = Integer.toString(time_pos);
            }
            if (bdir == 1) {
//          usacreport.FrmSacReport.jBook1.setActiveCell(row,pos);
               if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                   type.compareTo("yf") == 0
                   || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                   type.compareTo("yi") == 0
                   || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                   type.compareTo("yt") == 0
                   || type.compareTo("yv") == 0 || type.compareTo("y12") == 0 ||
                   type.compareTo("y13") == 0
                   || type.compareTo("y16") == 0 || type.compareTo("y17") == 0) {
                  Calendar caltemp1 = Calendar.getInstance(TimeZone.getDefault());
                  caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
                  Date dttemp = new Date();
                  long ltemp = (long) Double.parseDouble(str2);
                  ltemp = ltemp * (long) 86400000.0;
                  dttemp.setTime(ltemp);
                  caltemp1.setTime(dttemp);
                  int m_temp = (int) Double.parseDouble(str3);
                  int hour, min;
                  hour = m_temp / 60;
                  min = m_temp % 60;
                  String str_hour = "", str_min = "", str_day = "",
                      str_mon = "";
                  if (hour > 9) {
                     str_hour = Integer.toString(hour);
                  }
                  else {
                     str_hour = "0" + Integer.toString(hour);
                  }
                  if (min > 9) {
                     str_min = Integer.toString(min);
                  }
                  else {
                     str_min = "0" + Integer.toString(min);
                  }
                  if (caltemp1.get(caltemp1.MONTH) + 1 > 9) {
                     str_mon = Integer.toString(caltemp1.get(caltemp1.MONTH) +
                                                1);
                  }
                  else {
                     str_mon = "0" +
                         Integer.toString(caltemp1.get(caltemp1.MONTH) + 1);
                  }
                  if (caltemp1.get(caltemp1.DAY_OF_MONTH) > 9) {
                     str_day = Integer.toString(caltemp1.get(caltemp1.
                         DAY_OF_MONTH));
                  }
                  else {
                     str_day = "0" +
                         Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
                  }
                  usacreport.FrmSacReport.jBook1.setText(row, pos,
                      str_day + "-" + str_hour + ":" + str_min); //str_mon+"-"+
               }
               else {
                  usacreport.FrmSacReport.jBook1.setNumber(row, pos, m_result);
               }
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               usac_vec.row = row;
               usac_vec.col = pos;
               usac_vec.str_cell = s_cellvalue;
               veccontent.addElement(usac_vec);
            }
            else {
//              usacreport.FrmSacReport.jBook1.setActiveCell(pos,col);
               if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                   type.compareTo("yf") == 0
                   || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                   type.compareTo("yi") == 0
                   || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                   type.compareTo("yt") == 0
                   || type.compareTo("yv") == 0 || type.compareTo("y12") == 0 ||
                   type.compareTo("y13") == 0
                   || type.compareTo("y16") == 0 || type.compareTo("y17") == 0) {
                  Calendar caltemp1 = Calendar.getInstance(TimeZone.getDefault());
                  caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
                  Date dttemp = new Date();
                  long ltemp = (long) Double.parseDouble(str2);
                  ltemp = ltemp * (long) 86400000.0;
                  dttemp.setTime(ltemp);
                  caltemp1.setTime(dttemp);
                  int m_temp = (int) Double.parseDouble(str3);
                  int hour, min;
                  hour = m_temp / 60;
                  min = m_temp % 60;
                  String str_hour = "", str_min = "", str_day = "",
                      str_mon = "";
                  if (hour > 9) {
                     str_hour = Integer.toString(hour);
                  }
                  else {
                     str_hour = "0" + Integer.toString(hour);
                  }
                  if (min > 9) {
                     str_min = Integer.toString(min);
                  }
                  else {
                     str_min = "0" + Integer.toString(min);
                  }
                  if (caltemp1.get(caltemp1.MONTH) + 1 > 9) {
                     str_mon = Integer.toString(caltemp1.get(caltemp1.MONTH) +
                                                1);
                  }
                  else {
                     str_mon = "0" +
                         Integer.toString(caltemp1.get(caltemp1.MONTH) + 1);
                  }
                  if (caltemp1.get(caltemp1.DAY_OF_MONTH) > 9) {
                     str_day = Integer.toString(caltemp1.get(caltemp1.
                         DAY_OF_MONTH));
                  }
                  else {
                     str_day = "0" +
                         Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
                  }
//                usacreport.FrmSacReport.jBook1.setText(pos,col,str_hour + ":" + str_min);
                  usacreport.FrmSacReport.jBook1.setText(row, pos,
                      str_day + "-" + str_hour + ":" + str_min); //str_mon+"-"+
               }
               else {
                  usacreport.FrmSacReport.jBook1.setNumber(pos, col, m_result);
               }
               usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
               usac_vec.row = pos;
               usac_vec.col = col;
               usac_vec.str_cell = s_cellvalue;
               veccontent.addElement(usac_vec);
            }
            pos++;
            time_pos += 1;
         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }

      try {
         if (bdir == 1) {
            if (pos < col + 12) {
               for (i = pos; i < col + 12; i++) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0 ||
                      type.compareTo("y17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(row, i, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = i;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
         }
         else {
            if (pos < row + 12) {
               for (i = pos; i < row + 12; i++) {
                  if (type.compareTo("y2") == 0 || type.compareTo("y4") == 0 ||
                      type.compareTo("yf") == 0
                      || type.compareTo("yg") == 0 || type.compareTo("yh") == 0 ||
                      type.compareTo("yi") == 0
                      || type.compareTo("yq") == 0 || type.compareTo("yr") == 0 ||
                      type.compareTo("yt") == 0
                      || type.compareTo("yv") == 0 ||
                      type.compareTo("y12") == 0 || type.compareTo("y13") == 0
                      || type.compareTo("y16") == 0 ||
                      type.compareTo("y17") == 0) {
                     usacreport.FrmSacReport.jBook1.setText(i, col, "00-00:00");
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                  }
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(usac_vec);
               }
            }
         }
      }
      catch (com.f1j.util.F1Exception eii) {
      }
      return pos;
   }

   int year_single(String type, String name, String tabletime, int begin,
                   int end, int nowdate, int selhour, int row, int col) {
      String dbname = hdb_name;
      String tablename = "";
      int nFlag = 0;

      usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
      usac_vec.row = row;
      usac_vec.col = col;
      usac_vec.str_cell = s_cellvalue;
      veccontent.addElement(usac_vec);

      if (type.compareTo("za") == 0) {
         nFlag = 51;
      }
      else if (type.compareTo("zb") == 0) {
         nFlag = 52;
      }
      else if (type.compareTo("zc") == 0) {
         nFlag = 53;
      }
      else if (type.compareTo("z1") == 0 || type.compareTo("z2") == 0) {
         nFlag = 55;
      }
      else if (type.compareTo("z3") == 0 || type.compareTo("z4") == 0) {
         nFlag = 56;
      }
      else if (type.compareTo("z5") == 0) {
         nFlag = 57;
      }
      else if (type.compareTo("z6") == 0) {
         nFlag = 58;
      }
      else if (type.compareTo("z7") == 0) {
         nFlag = 59;
      }
      else if (type.compareTo("z8") == 0) {
         nFlag = 62;
      }
      else if (type.compareTo("z9") == 0) {
         nFlag = 63;
      }
      else if (type.compareTo("zd") == 0) {
         nFlag = 64;
      }
      else if (type.compareTo("ze") == 0) {
         nFlag = 65;
      }
      else if (type.compareTo("zf") == 0) {
         nFlag = 66;
      }
      else if (type.compareTo("zg") == 0) {
         nFlag = 67;
      }
      else if (type.compareTo("zh") == 0) {
         nFlag = 68;
      }
      else if (type.compareTo("zi") == 0) {
         nFlag = 69;
      }
      else if (type.compareTo("zj") == 0) {
         nFlag = 70;
      }
      else if (type.compareTo("zk") == 0) {
         nFlag = 71;
      }
      else if (type.compareTo("zl") == 0) {
         nFlag = 72;
      }
      else if (type.compareTo("zm") == 0) {
         nFlag = 73;
      }
      else if (type.compareTo("zn") == 0) {
         nFlag = 74;
      }
      else if (type.compareTo("zo") == 0) {
         nFlag = 75;
      }
      else if (type.compareTo("zp") == 0) {
         nFlag = 79;
      }
      else if (type.compareTo("zq") == 0) {
         nFlag = 80;
      }
      else if (type.compareTo("zr") == 0) {
         nFlag = 81;
      }
      else if (type.compareTo("zs") == 0 || type.compareTo("zt") == 0) {
         nFlag = 150;
      }
      else if (type.compareTo("zu") == 0 || type.compareTo("zv") == 0) {
         nFlag = 151;
      }
      else if (type.compareTo("zw") == 0) {
         nFlag = 152;
      }
      else if (type.compareTo("zx") == 0) {
         nFlag = 153;
      }
      else if (type.compareTo("zy") == 0) {
         nFlag = 154;
      }
      else if (type.compareTo("zz") == 0) {
         nFlag = 158;
      }
      else if (type.compareTo("z11") == 0) {
         nFlag = 159;
      }
      else if (type.compareTo("z12") == 0) {
         nFlag = 162;
      }
      else if (type.compareTo("z13") == 0) {
         nFlag = 163;
      }
      else if (type.compareTo("z14") == 0) {
         nFlag = 165;
      }
      else if (type.compareTo("z15") == 0) {
         nFlag = 166;
      }
      else if (type.compareTo("z16") == 0) {
         nFlag = 167;
      }
      else if (type.compareTo("z17") == 0) {
         nFlag = 168;
      }
      else if (type.compareTo("z18") == 0) {
         nFlag = 169;
      }
      else if (type.compareTo("z19") == 0) {
         nFlag = 170;
      }
      else if (type.compareTo("z20") == 0) {
         nFlag = 171;
      }
      else if (type.compareTo("z21") == 0) {
         nFlag = 172;
      }
      else if (type.compareTo("z22") == 0) {
         nFlag = 180;
      }
      else if (type.compareTo("z23") == 0) {
         nFlag = 200;
      }
      else if (type.compareTo("z24") == 0) {
         nFlag = 201;
      }
      else {
         return -1;
      }

      if (nFlag == 51 || nFlag == 52 || nFlag == 53 || nFlag == 79 ||
          nFlag == 70 || nFlag == 71
          || nFlag == 72 || nFlag == 73 || nFlag == 74 || nFlag == 75 ||
          nFlag == 170
          || nFlag == 171 || nFlag == 172 || nFlag == 180) {
         tablename = "yearkwh" + tabletime;
      }
      else {
         tablename = "yearyc" + tabletime;
      }
      String fieldname = "";
      fieldname = "data,odate,otime";

      String where = "", order = "";
      where = "name = '" + name + "' and flag = " + Integer.toString(nFlag);

//    System.out.println("年单点数据检索条件为:" + where);
      int[] icount = new int[1];
      int i = 0, j = 0;
      int pos = row;
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      try {
         usacreport.FrmSacReport.jBook1.setActiveCell(row, col);
      }
      catch (com.f1j.util.F1Exception ea) {}
      if (returnv <= 0) {
         try {
            if (type.compareTo("z2") == 0 || type.compareTo("z4") == 0 ||
                type.compareTo("zf") == 0
                || type.compareTo("zg") == 0 || type.compareTo("zh") == 0 ||
                type.compareTo("zi") == 0
                || type.compareTo("zq") == 0 || type.compareTo("zr") == 0 ||
                type.compareTo("zt") == 0
                || type.compareTo("zv") == 0 || type.compareTo("z12") == 0 ||
                type.compareTo("z13") == 0
                || type.compareTo("z16") == 0 || type.compareTo("z17") == 0) {
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            1);
               usacreport.FrmSacReport.jBook1.setCellText("00:00");
            }
            else {
               fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col,
                            0);
               usacreport.FrmSacReport.jBook1.setNumber(0.0000);
            }
            return row;
         }
         catch (com.f1j.util.F1Exception e) {
         }
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      double m_result = 0.0;

      if (type.compareTo("z2") == 0 || type.compareTo("z4") == 0 ||
          type.compareTo("zf") == 0
          || type.compareTo("zg") == 0 || type.compareTo("zh") == 0 ||
          type.compareTo("zi") == 0
          || type.compareTo("zq") == 0 || type.compareTo("zr") == 0 ||
          type.compareTo("zt") == 0
          || type.compareTo("zv") == 0 || type.compareTo("z12") == 0 ||
          type.compareTo("z13") == 0
          || type.compareTo("z16") == 0 || type.compareTo("z17") == 0) { //对于年报表的时间不是很有用
         try {
            fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 1);
            String str2 = new String(buffer[1]); //odate
            str2 = str2.trim();
            String str1 = new String(buffer[2]); //otime
            str1 = str1.trim();
            Calendar caltemp1 = Calendar.getInstance(TimeZone.getDefault());
            caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
            Date dttemp = new Date();
            long ltemp = (long) Double.parseDouble(str2);
            ltemp = ltemp * (long) 86400000.0;
            dttemp.setTime(ltemp);
            caltemp1.setTime(dttemp);
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.YEAR))+ "年");
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.MONTH)+1));
            //System.out.println(Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH)));
            int m_result1 = (int) Double.parseDouble(str1);
            int hour, min;
            hour = m_result1 / 60;
            min = m_result1 % 60;
            String str_hour = "", str_min = "", str_day = "", str_mon = "";
            if (hour > 9) {
               str_hour = Integer.toString(hour);
            }
            else {
               str_hour = "0" + Integer.toString(hour);
            }
            if (min > 9) {
               str_min = Integer.toString(min);
            }
            else {
               str_min = "0" + Integer.toString(min);
            }
            if (caltemp1.get(caltemp1.MONTH) + 1 > 9) {
               str_mon = Integer.toString(caltemp1.get(caltemp1.MONTH) + 1);
            }
            else {
               str_mon = "0" +
                   Integer.toString(caltemp1.get(caltemp1.MONTH) + 1);
            }
            if (caltemp1.get(caltemp1.DAY_OF_MONTH) > 9) {
               str_day = Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
            }
            else {
               str_day = "0" +
                   Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
            }
            //usacreport.FrmSacReport.jBook1.setCellText(str_mon+"-"+str_day+"-"+str_hour + ":" + str_min);//jhongtu
            usacreport.FrmSacReport.jBook1.setCellText(Integer.toString(
                caltemp1.get(caltemp1.YEAR)) + "-" + str_mon + "-" + str_day +
                " " + str_hour + ":" + str_min);
         }
         catch (com.f1j.util.F1Exception e3) {
         }
      }
      else {
         try {
            fillcellpara(name, tablename, nFlag, nowdate, 0, type, row, col, 0);
            String str1 = new String(buffer[0]); //data
            str1 = str1.trim();
            m_result = Double.parseDouble(str1);
            if (usacreport.FrmSacReport.babs) {
               if (m_result < 0) {
                  m_result = Math.abs(m_result);
               }
            }
            usacreport.FrmSacReport.jBook1.setNumber(m_result);
         }
         catch (com.f1j.util.F1Exception e2) {
         }
      }
      return pos;
   }

   void data_proc() {
      int i, j;
      bData = true;
      veccontent.removeAllElements();
      try {
         usacreport.FrmSacReport.jBook1.initWorkbook();
         String filesp = System.getProperty("file.separator");
         String path = "";
         if (out_filename.length() > 0) {
            path = usacreport.FrmSacReport.s_sacenv + filesp +
                "reportfile" + filesp + in_filenameid + ".vts";
         }
         else {
            path = usacreport.FrmSacReport.s_sacenv + filesp +
                "reportfile" + filesp + g_selfilename + ".vts";
         }

         usacreport.FrmSacReport.vectorpara.removeAllElements();
         usacreport.FrmSacReport.jBook1.read(path); //读入报表模板


         String s_type = "", s_name = "", s_time = "", s_hour = "",
             tabletime = "";
         int pos = 0;

         if (cal_dt1.get(cal_dt1.MONTH) >= 9) {
            if (cal_dt.get(cal_dt.DAY_OF_MONTH) < 10) {
               usacreport.FrmSacReport.g_strsdate = Integer.toString(cal_dt.get(
                   cal_dt.YEAR)) +
                   Integer.toString(cal_dt1.get(cal_dt1.MONTH) + 1) + "0" +
                   Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
            }
            else {
               usacreport.FrmSacReport.g_strsdate = Integer.toString(cal_dt.get(
                   cal_dt.YEAR)) +
                   Integer.toString(cal_dt1.get(cal_dt1.MONTH) + 1) +
                   Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
            }
         }
         else {
            if (cal_dt.get(cal_dt.DAY_OF_MONTH) < 10) {
               usacreport.FrmSacReport.g_strsdate = Integer.toString(cal_dt.get(
                   cal_dt.YEAR)) + "0" +
                   Integer.toString(cal_dt1.get(cal_dt1.MONTH) + 1) + "0" +
                   Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
            }
            else {
               usacreport.FrmSacReport.g_strsdate = Integer.toString(cal_dt.get(
                   cal_dt.YEAR)) + "0" +
                   Integer.toString(cal_dt1.get(cal_dt1.MONTH) + 1) +
                   Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
            }
         }

         mon_delrowcol = true;

         //lcg 20090814增加工作表操作
         for(int sheetNum=0;sheetNum<usacreport.FrmSacReport.jBook1.getNumSheets();sheetNum++){
            usacreport.FrmSacReport.jBook1.setSheet(sheetNum);
            if(PreProcessor.CMDLINE_DEBUG)
            {
               PreProcessor.WriteLogFile("正在处理sheet索引" + String.valueOf(sheetNum));
            }
            for (i = 0; i <= usacreport.FrmSacReport.jBook1.getLastRow(); i++) {
               for (j = 0; j <= usacreport.FrmSacReport.jBook1.getLastCol(); j++) {
                  s_cellvalue = usacreport.FrmSacReport.jBook1.getText(i, j).trim();
                  if (s_cellvalue.length() <= 3) {
                     continue;
                  }
                  if (s_cellvalue.substring(0, 1).compareTo("@") == 0) {
                     //处理SQL语句显示数据
                     if (s_cellvalue.length() >= 4 &&
                         s_cellvalue.substring(1, 4).compareToIgnoreCase("SQL") ==
                         0) {
                        String strTemp = "", strSQL = "";
                        pos = s_cellvalue.indexOf(',');
                        strTemp = s_cellvalue.substring(pos + 1,
                                                        s_cellvalue.length());
                        strSQL = strTemp;
                        SQL_proc(strSQL, cal_dt1, cal_dt2, cal_dt, i, j);
                        continue;
                     }

                     //下面处理数据类型
                     if (s_cellvalue.substring(1, 4).compareTo("nyr") == 0) { //处理日报表的日期显示
                        usacreport.FrmSacReport.jBook1.setText(i, j,
                            Integer.toString(cal_dt.get(cal_dt.YEAR)) + "-" +
                            Integer.toString(cal_dt.get(cal_dt.MONTH) + 1) + "-" +
                            Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH)));
                        //usacreport.FrmSacReport.jBook1.setText(i,j,jComyear.getSelectedItem() + "-" +
                        //    jCommon.getSelectedItem() + "-" + jComday.getSelectedItem());
                        continue;
                     }
                     if (s_cellvalue.substring(1, 4).compareTo("ny1") == 0) { //处理月报表的日期显示
                        usacreport.FrmSacReport.jBook1.setText(i, j,
                            Integer.toString(cal_dt.get(cal_dt.YEAR)) + "-" +
                            Integer.toString(cal_dt.get(cal_dt.MONTH) + 1));

                        continue;
                     }

                     if (s_cellvalue.substring(1, 4).compareTo("ny2") == 0) { //处理年报表的日期显示
                        usacreport.FrmSacReport.jBook1.setText(i, j,
                            Integer.toString(cal_dt.get(cal_dt.YEAR)));
                        continue;
                     }

                     String s_valuetemp = s_cellvalue;
                     pos = s_cellvalue.indexOf(',');
                     s_type = s_cellvalue.substring(1, pos);

                     //南网测试，处理实时报表格式---20120405-hongxiang-start
                     if (s_type.compareTo("rtdb") == 0) {
                     s_valuetemp = s_valuetemp.substring(pos + 1,s_valuetemp.length());
                     pos = s_valuetemp.indexOf(',');

                     String s_tablename=s_valuetemp.substring(0, pos);
                     s_valuetemp = s_valuetemp.substring(pos + 1,s_valuetemp.length());

                     pos = s_valuetemp.indexOf(',');
                     String s_sname=s_valuetemp.substring(0, pos);

                     String s_field = s_valuetemp.substring(pos + 1,s_valuetemp.length());

                     byte[] value = usacreport.FrmSacReport.f1jdll.readRtdbRecorders(s_tablename.getBytes(),
                                              s_sname.getBytes(),s_field.getBytes());
                        String svalue = new String(value);
                        usacreport.FrmSacReport.jBook1.setText(i, j, svalue);
                        continue;
                     }
                     //南网测试，处理实时报表格式---20120405-hongxiang-start

                     s_valuetemp = s_valuetemp.substring(pos + 1,
                                                         s_valuetemp.length());
                     pos = s_valuetemp.indexOf(',');
                     s_name = s_valuetemp.substring(0, pos);

                     s_hour = s_valuetemp.substring(pos + 1, s_valuetemp.length());
                     if (cal_dt.get(cal_dt.MONTH) < 9) { /////表的日期
                        tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)) +
                            "0" + Integer.toString(cal_dt.get(cal_dt.MONTH) + 1);
                     }
                     else {
                        tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)) +
                            Integer.toString(cal_dt.get(cal_dt.MONTH) + 1);
                     }

                     if (s_type.compareTo("31") == 0 ||
                         s_type.compareTo("32") == 0) { //月中每一天的整点数据
                        //int total_day1 = (int)(cal_dt1.getTime().getTime()/1000); //jhongtu -- 20060909
                        int total_day1 = (int) (cal_dt1.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day1 = total_day1 / 86400;
                        //int total_day2 = (int)(cal_dt2.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day2 = (int) (cal_dt2.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day2 = total_day2 / 86400;
                        data_31(s_type, s_name, tabletime, total_day1, total_day2,
                                Integer.parseInt(s_hour), i, j); //以后再处理格子的跳转问题
                     }
                     if (s_type.compareTo("xd") == 0 ||
                         s_type.compareTo("xh") == 0) { //处理相关日表
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;

                        int retrow = xgd_proc(s_type, s_name, tabletime, 0, 0,
                                              total_day, 0, i, j); //以后再处理格子的跳转问题
                     }
                     else if (s_type.compareTo("xm") == 0 ||
                              s_type.compareTo("xn") == 0) { //处理相关月表

                     }
                     else if (s_type.compareTo("xy") == 0 ||
                              s_type.compareTo("xz") == 0) { //处理相关年表(复杂待处理)

                     }
                     else if (s_type.compareTo("emax") == 0 ||
                              s_type.compareTo("emin") == 0) {
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;
                        int retrow = m_proc(s_type, s_name, tabletime, 0, 0,
                                            total_day, 0, i, j); //以后再处理格子的跳转问题
                     }
                     else if (s_type.compareTo("dh") == 0 ||
                              s_type.compareTo("dd") == 0 ||
                              s_type.compareTo("db") == 0 ||
                              s_type.compareTo("maxtime") == 0 ||
                              s_type.compareTo("mintime") == 0) { //24点遥测和电度量值
                        //  int total_day = (int)(cal_dt.getTime().getTime()/1000)   ;//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;

                        /*  String s1 = "1970-01-01";
                         SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd",Locale.CHINA);
                          ParsePosition pos1 = new ParsePosition(0);
                          java.util.Date dt1=formatter.parse(s1,pos1);
                          String jct=jComyear.getSelectedItem()+"-"+jCommon.getSelectedItem()+"-"+jComday.getSelectedItem();
                          ParsePosition pos3 = new ParsePosition(0);
                          java.util.Date dt4=formatter.parse(jct,pos3);
                         long l3 = (dt4.getTime() - dt1.getTime())/(3600*24*1000);*/

                         //add by hongxiang 20170117 如果历史表分区分表，那么日表需要检索日表的数据
                         if(s_type.compareTo("db") != 0){ //电度表没有做分区分表处理
                             if (ConstantDef.IF_HISDB_PARTITION){
                                 if (cal_dt.get(cal_dt.DAY_OF_MONTH) <= 9) { /////表的日期
                                     tabletime = tabletime + "0" + Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
                                 }else{
                                     tabletime = tabletime + Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
                                 }
                             }
                         }

                        int retrow = dh_proc(s_type, s_name, tabletime, 0, 0,
                                             total_day, Integer.parseInt(s_hour),
                                             i, j); //以后再处理格子的跳转问题
                     }
                     else if (s_type.substring(0, 1).compareTo("e") == 0) { //日统计数据
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;

                        int retrow = day_single(s_type, s_name, tabletime, 0, 0,
                                                total_day, Integer.parseInt(s_hour), i, j); //以后再处理格子的跳转问题
                     }

                     else if (s_type.substring(0, 2).compareTo("dk") == 0 ||
                              s_type.substring(0, 2).compareTo("dk") == 0) {

                     }
                     else if ( (s_type.substring(0, 2).compareTo("mk") == 0 ||
                                s_type.substring(0, 2).compareTo("mt") == 0) &&
                              s_type.length() > 2
                         ) { //20081129添加开关状态处理,和通道月单点数据处理函数，放到月数据处理之前；20090226这里有可能和月数据处理冲突，因此加入了类型长度的限制
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;
                        //int total_day1 = (int)(cal_dt1.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day1 = (int) (cal_dt1.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day1 = total_day1 / 86400;
                        //int total_day2 = (int)(cal_dt2.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day2 = (int) (cal_dt2.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day2 = total_day2 / 86400;
                        //tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)); //month2003
                        int retrow = mon_K_single(s_type, s_name, tabletime,
                                                  total_day1,
                                                  total_day2, total_day, 0, i, j); //以后再处理格子的跳转问题
                     }
                     else if (s_type.substring(0, 1).compareTo("m") == 0) { //月规律数据
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu --20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++20060909
                        total_day = total_day / 86400;
                        //int total_day1 = (int)(cal_dt1.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day1 = (int) (cal_dt1.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day1 = total_day1 / 86400;
                        //int total_day2 = (int)(cal_dt2.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day2 = (int) (cal_dt2.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day2 = total_day2 / 86400;
                        int retrow = mon_gl(s_type, s_name, tabletime, total_day1,
                                            total_day2, total_day,
                                            Integer.parseInt(s_hour), i, j); //以后再处理格子的跳转问题
                     }

                     else if (s_type.substring(0, 1).compareTo("n") == 0 ||
                              s_type.substring(0, 1).compareTo("x") == 0
                         ) { //旬、月统计数据
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;
                        //int total_day1 = (int)(cal_dt1.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day1 = (int) (cal_dt1.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day1 = total_day1 / 86400;
                        //int total_day2 = (int)(cal_dt2.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day2 = (int) (cal_dt2.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day2 = total_day2 / 86400;
                        tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)); //month2003
                        int retrow = mon_single(s_type, s_name, tabletime,
                                                total_day1, total_day2, total_day,
                                                0, i, j); //以后再处理格子的跳转问题

                     }
                     else if (s_type.substring(0, 1).compareTo("y") == 0) { //年规律数据
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;
                        int iiii = cal_dt1.YEAR;
                        int total_day1 = (int) cal_dt1.get( (cal_dt1.YEAR)) - 1970;
                        total_day1 = total_day1 * 12 + 1;
                        int total_day2 = (int) cal_dt2.get(cal_dt2.YEAR) - 1970 +
                            1;
                        total_day2 = total_day2 * 12 + 1;
                        tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)); //month2003
                        int retrow = year_gl(s_type, s_name, tabletime, total_day1,
                                             total_day2, total_day, 0, i, j); //以后再处理格子的跳转问题
                     }
                     else if (s_type.substring(0, 1).compareTo("z") == 0) { //年统计数据
                        //int total_day = (int)(cal_dt.getTime().getTime()/1000);//jhongtu -- 20060909
                        int total_day = (int) (cal_dt.getTime().getTime() / 1000) +
                            8 * 60 * 60; //jhongtu ++ 20060909
                        total_day = total_day / 86400;
                        int total_day1 = (int) cal_dt1.get( (cal_dt1.YEAR)) - 1970;
                        total_day1 = total_day1 * 12 + 1;
                        int total_day2 = (int) cal_dt2.get(cal_dt2.YEAR) - 1970 +
                            1;
                        total_day2 = total_day2 * 12 + 1;
                        tabletime = Integer.toString(cal_dt.get(cal_dt.YEAR)); //year2003
                        int retrow = year_single(s_type, s_name, tabletime,
                                                 total_day1, total_day2, total_day,
                                                 0, i, j); //以后再处理格子的跳转问题
                     }
                     else {
                        continue;
                     }
                  }
               }
            }
         }
         mon_delrowcol = false;
      }
      catch (com.f1j.util.F1Exception e1) {
      }
      catch (java.io.IOException e2) {
      }

      //在这里处理临时表列和表行，以及临时的单元格

      if (out_filename.length() > 0) {
         if (out_filename.compareTo("print") == 0) {
//        System.out.println("打印!");
            try {
//          usacreport.FrmSacReport.jBook1.setPrintArea("$A$1:"+usacreport.FrmSacReport.jBook1.formatRCNr(usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getLastCol(),true));
               usacreport.FrmSacReport.jBook1.setPrintVCenter(true); //报表Y轴居中
               usacreport.FrmSacReport.jBook1.setPrintHCenter(true); //报表X轴居中
               usacreport.FrmSacReport.jBook1.setPrintScaleFitToPage(true);
               usacreport.FrmSacReport.jBook1.setPrintHeader("");
               usacreport.FrmSacReport.jBook1.setPrintFooter("");
               usacreport.FrmSacReport.jBook1.setPrintScale(400, true, 1, 1); //400 fit to hole page
                if ("1".compareTo((String) vectorprint.get(vecid + 3)) == 0) {
                usacreport.FrmSacReport.jBook1.setPrintLandscape(true); //true为横向
                              }
                              else {
                usacreport.FrmSacReport.jBook1.setPrintLandscape(false); //false为竖纸
                              }
                              short papersize = 9;
                if ("1".compareTo( (String) vectorprint.get(vecid + 2)) == 0) {
                                 papersize = 8;
                usacreport.FrmSacReport.jBook1.setPrintPaperSize(papersize);
                              }
                              else {
                usacreport.FrmSacReport.jBook1.setPrintPaperSize(papersize);
                              }

               usacreport.FrmSacReport.jBook1.setPrintNumberOfCopies(1);
               usacreport.FrmSacReport.jBook1.filePrint(false);
            }
            catch (com.f1j.util.F1Exception e3) {
            }

         }
         else if (out_filename.compareTo("autoprint") == 0) {
      //        System.out.println("打印!");
            try {
      //          usacreport.FrmSacReport.jBook1.setPrintArea("$A$1:"+usacreport.FrmSacReport.jBook1.formatRCNr(usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getLastCol(),true));
               usacreport.FrmSacReport.jBook1.setPrintVCenter(true); //报表Y轴居中
               usacreport.FrmSacReport.jBook1.setPrintHCenter(true); //报表X轴居中
               usacreport.FrmSacReport.jBook1.setPrintScaleFitToPage(true);
               usacreport.FrmSacReport.jBook1.setPrintHeader("");
               usacreport.FrmSacReport.jBook1.setPrintFooter("");
               usacreport.FrmSacReport.jBook1.setPrintScale(400, true, 1, 1); //400 fit to hole page
               /*
                if ("1".compareTo((String) vectorprint.get(vecid + 3)) == 0) {
                usacreport.FrmSacReport.jBook1.setPrintLandscape(true); //true为横向
                              }
                              else {
                usacreport.FrmSacReport.jBook1.setPrintLandscape(false); //false为竖纸
                              }
                              short papersize = 9;
                if ("1".compareTo( (String) vectorprint.get(vecid + 2)) == 0) {
                                 papersize = 8;
                usacreport.FrmSacReport.jBook1.setPrintPaperSize(papersize);
                              }
                              else {
                usacreport.FrmSacReport.jBook1.setPrintPaperSize(papersize);
                              }
                */
              if( print_type.compareTo("1") == 0)
                 usacreport.FrmSacReport.jBook1.setPrintLandscape(false); //false为竖纸
              else
                  usacreport.FrmSacReport.jBook1.setPrintLandscape(true); //false为竖纸
               short papersize = 9;
               usacreport.FrmSacReport.jBook1.setPrintPaperSize(papersize);
               usacreport.FrmSacReport.jBook1.setPrintNumberOfCopies(1);
               usacreport.FrmSacReport.jBook1.filePrint(false);
            }
            catch (com.f1j.util.F1Exception e3) {
            }

         }

         else {
            if(PreProcessor.CMDLINE_DEBUG==true){
               PreProcessor.WriteLogFile("开始生成html和xls文件");
            }
            htmlgen();
            xlsgen();
         }
      }
   }

   //修改日期，当选择日期后调用
   void fill_cal() {
      //cal_dt设置为界面上选择的日期。
      if(PreProcessor.UseSelHourTime)
         cal_dt.set(jComyear.getSelectedIndex() + 2000,
                 jCommon.getSelectedIndex(), jComday.getSelectedIndex() + 1,jComHour.getSelectedIndex(),0);
      else
         if(PreProcessor.PWTEST)
            cal_dt.set(jComyear.getSelectedIndex() + 2000,
                 jCommon.getSelectedIndex(), jComday.getSelectedIndex() + 1,21,0);
         else
            cal_dt.set(jComyear.getSelectedIndex() + 2000,
                 jCommon.getSelectedIndex(), jComday.getSelectedIndex() + 1,0,0);
      //cal_dt1被设置为当前选择日期，日字段设置为1，即本月1日。
      cal_dt1.set(jComyear.getSelectedIndex() + 2000,
                  jCommon.getSelectedIndex(), 1);
      //cal_dt2被设置为当前选择日期，月字段设置为下月，日字段设置为1，即下月1日。
      if (jCommon.getSelectedIndex() == 11) {
         cal_dt2.set(jComyear.getSelectedIndex() + 2001,
                     0, 1);
      }
      else {
         cal_dt2.set(jComyear.getSelectedIndex() + 2000,
                     jCommon.getSelectedIndex() + 1, 1);
      }
   }

   void fill_calcmd() {
      if(PreProcessor.UseSelHourTime)
      {
         cal_dt.set(Integer.parseInt(g_year),
                    Integer.parseInt(g_mon) - 1, Integer.parseInt(g_day),
                    Integer.parseInt(g_hour), 0);
      }
      else
      {
         if(PreProcessor.PWTEST)
            cal_dt.set(Integer.parseInt(g_year),
                     Integer.parseInt(g_mon) - 1, Integer.parseInt(g_day),
                     21, 0);

         else
            cal_dt.set(Integer.parseInt(g_year),
                       Integer.parseInt(g_mon) - 1, Integer.parseInt(g_day),
                       0, 0);

      }

      cal_dt1.set(Integer.parseInt(g_year),
                  Integer.parseInt(g_mon) - 1, 1);
      if (g_mon.compareTo("12") == 0) {
         cal_dt2.set(Integer.parseInt(g_year) + 1,
                     0, 1);
      }
      else {
         cal_dt2.set(Integer.parseInt(g_year),
                     Integer.parseInt(g_mon), 1);
      }

      //System.out.println(cal_dt.get(cal_dt.YEAR) + "::" + cal_dt.get(cal_dt.MONTH) + "::" + cal_dt.get(cal_dt.DAY_OF_MONTH));
      //System.out.println(cal_dt1.get(cal_dt.YEAR) + "::" + cal_dt1.get(cal_dt.MONTH) + "::" + cal_dt1.get(cal_dt.DAY_OF_MONTH));
      //System.out.println(cal_dt2.get(cal_dt.YEAR) + "::" + cal_dt2.get(cal_dt.MONTH) + "::" + cal_dt2.get(cal_dt.DAY_OF_MONTH));
      //System.out.println("end!");
   }

   void htmlgen() {
      com.f1j.ss.HTMLWriter htmlwriter = new com.f1j.ss.HTMLWriter();

//    System.out.println("进入html生成程序!");
      try {
         String filesp = System.getProperty("file.separator");
         String filename;
         // filename = usacreport.FrmSacReport.s_sacenv + filesp + "jakarta-tomcat/webapps/jsp-examples/html" + filesp + out_filename;
//      filename = "/usr/jakarta-tomcat/webapps/jsp-examples/html" + filesp +  out_filename;//nt
         // String os = System.getProperty("os.name");
         //if (os.startsWith("Windows"))
         //{
         //  filename = "c:/Tomcat 5.0/webapps/num/html" + filesp + out_filename;
         //}else{
         //filename = usacreport.FrmSacReport.s_sacenv + filesp + "jakarta-tomcat/webapps/num/xls" + filesp +  out_filename;
         //filename = usacreport.FrmSacReport.s_sacenv + filesp + "html" + filesp +  out_filename;
         //}

         filename = usacreport.FrmSacReport.s_sacenv + filesp + "html" + filesp +
             out_filename;

         int pos = filename.lastIndexOf(".");
         if (pos < 0) {
            filename = filename + ".html";
         }
         else {
            filename = filename.substring(0, pos);
            filename = filename + ".html";
         }
//      System.out.println(filename);
         java.io.FileWriter iowriter = new java.io.FileWriter(filename);
         htmlwriter.setFlags(htmlwriter.ALL);
         usacreport.FrmSacReport.jBook1.setShowGridLines(false);
         htmlwriter.write(usacreport.FrmSacReport.jBook1.getBook(), iowriter);
      }
      catch (java.io.IOException eee) {
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void xlsgen() {
      try {
         String filesp = System.getProperty("file.separator");
         String filename;
//      filename = usacreport.FrmSacReport.s_sacenv + filesp + "reportfile" + filesp + out_filename;//nt
         /**      filename = "/usr/jakarta-tomcat/webapps/jsp-examples/html" + filesp +  out_filename;//nt
               int pos = filename.lastIndexOf(".");
               if(pos < 0){
                 filename = filename + ".xls";
               }else{
                 filename = filename.substring(0,pos);
                 filename = filename + ".xls";
               }*/
         //\4,2004,08,28,sjfdslfjs.xls
         String str_time = "";
         if (out_filename.length() <= 0) {
            return;
         }
//      str_time = out_filename.substring(out_filename.length()-13,out_filename.length()-5);
         //     filename = usacreport.FrmSacReport.s_sacenv + filesp + "jakarta-tomcat/webapps/num/xls" + filesp +  in_filenameid + str_time;
//      filename = "/usr/apache/htdocs/manual/xls" + filesp +  out_filename;
         //filename = usacreport.FrmSacReport.s_sacenv + filesp + "xls" + filesp +  out_filename;
         //   filename = usacreport.FrmSacReport.s_sacenv + filesp + "jakarta-tomcat/webapps/num/xls" + filesp +  out_filename;

         /* String os = System.getProperty("os.name");
          if (os.startsWith("Windows"))
          {
          filename = "c:/Tomcat 5.0/webapps/num/xls" + filesp +  out_filename;
          }else{
            //filename = usacreport.FrmSacReport.s_sacenv + filesp + "jakarta-tomcat/webapps/num/xls" + filesp +  out_filename;
            filename = usacreport.FrmSacReport.s_sacenv + filesp + "xls" + filesp +  out_filename;
          }
          */

         filename = usacreport.FrmSacReport.s_sacenv + filesp + "xls" + filesp +
             out_filename;
         int pos = filename.lastIndexOf(".");
         if (pos < 0) {
            filename = filename + ".xls";
         }
         else {
            filename = filename.substring(0, pos);
            filename = filename + ".xls";
         }

         /*      if(out_filename.length() > 0){
                 usacreport.FrmSacReport.jBook1.setColWidth(0,usacreport.FrmSacReport.jBook1.getLastCol(),usacreport.FrmSacReport.jBook1.getColWidth(0)*usacreport.FrmSacReport.g_gridrated/usacreport.FrmSacReport.g_gridrateu,false);
                 usacreport.FrmSacReport.jBook1.setRowHeight(0,usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getRowHeight(0)*usacreport.FrmSacReport.g_gridrated/usacreport.FrmSacReport.g_gridrateu,false,false);
               }*/

         java.io.File file = new java.io.File(filename);
         FileOutputStream outFile = new FileOutputStream(file);

         usacreport.FrmSacReport.jBook1.write(outFile,
                                              usacreport.FrmSacReport.jBook1.
                                              eFileExcel97);
      }
      catch (com.f1j.util.F1Exception e) {
      }
      catch (java.io.IOException e1) {
      }
      catch (java.lang.NullPointerException e2) {
      }
   }

   void fillcellpara(String name, String dbname, int flag, int sdate, int time,
                     String style, int row, int col, int type) {
      int i = 0, j = 0;
      if (style.compareTo("dh") == 0 || style.compareTo("dd") == 0) {
         if (time == 0) {
            time = 1;
            if (type == 0) {
               for (i = row; i < row + 25; i++) {
                  usacreport.cellpara cellpara2 = new usacreport.cellpara();
                  cellpara2.style = style;
                  cellpara2.name = name;
                  cellpara2.databasename = dbname;
                  cellpara2.flag = 1; //<50即不设置flag的条件
                  cellpara2.sdate = sdate;
                  cellpara2.time = time * 60;
                  time++;
                  cellpara2.type = 0;
                  cellpara2.col = col; //现在考虑col固定
                  cellpara2.row = i;
                  usacreport.FrmSacReport.vectorpara.addElement(cellpara2);
               }
               return;
            }
            else {
               for (i = col; i < col + 25; i++) {
                  usacreport.cellpara cellpara2 = new usacreport.cellpara();
                  cellpara2.style = style;
                  cellpara2.name = name;
                  cellpara2.databasename = dbname;
                  cellpara2.flag = 5; //<50即不设置flag的条件
                  cellpara2.sdate = sdate;
                  cellpara2.time = time * 60;
                  time++;
                  cellpara2.type = 0;
                  cellpara2.col = i; //现在考虑col固定
                  cellpara2.row = row;
                  usacreport.FrmSacReport.vectorpara.addElement(cellpara2);
               }
            }
            return;
         }
         else {
            usacreport.cellpara cellpara2 = new usacreport.cellpara();
            cellpara2.style = style;
            cellpara2.name = name;
            cellpara2.databasename = dbname;
            cellpara2.flag = 5; //<50即不设置flag的条件
            cellpara2.sdate = sdate;
            cellpara2.time = time * 60;
            cellpara2.type = 0;
            cellpara2.col = col; //现在考虑col固定
            cellpara2.row = row;
            usacreport.FrmSacReport.vectorpara.addElement(cellpara2);
            return;
         }
      }

      /*    if(style.substring(0,1).compareTo("e") == 0){
           usacreport.cellpara cellpara1 = new usacreport.cellpara();
           cellpara1.style = style;
           cellpara1.name = name;
           cellpara1.databasename = dbname;
           cellpara1.flag = flag;
           cellpara1.sdate = sdate;//这里是odate
           cellpara1.time = time;
           cellpara1.col = col;//现在考虑col固定cellpara1.row = i;
           cellpara1.row = row;
           cellpara1.type = type;
           usacreport.FrmSacReport.vectorpara.addElement(cellpara1);
           return;
         }
         return ;//只作日报表修改
        if(style.substring(0,1).compareTo("m") == 0){
           for(i = row;i<row + 32;i++){//如果这样处理,那么删除行的操作会产生错误,那暂时考虑不删除行
             usacreport.cellpara cellpara1 = new usacreport.cellpara();
             cellpara1.style = style;
             cellpara1.name = name;
             cellpara1.databasename = dbname;
             cellpara1.flag = flag;
             cellpara1.sdate = sdate++;
             cellpara1.time = time;
             cellpara1.type = 0;
             cellpara1.col = col;//现在考虑col固定
             cellpara1.row = i;
             usacreport.FrmSacReport.vectorpara.addElement(cellpara1);
           }
           return;
         }

         if(style.substring(0,1).compareTo("n") == 0){
           usacreport.cellpara cellpara1 = new usacreport.cellpara();
           cellpara1.style = style;
           cellpara1.name = name;
           cellpara1.databasename = dbname;
           cellpara1.flag = flag;
           cellpara1.sdate = sdate;
           cellpara1.time = time;
           cellpara1.col = col;//现在考虑col固定cellpara1.row = i;
           cellpara1.row = row;
           cellpara1.type = type;
//      System.out.println("n1");
           usacreport.FrmSacReport.vectorpara.addElement(cellpara1);
           return;
         }

         if(style.substring(0,1).compareTo("y") == 0){
           for(i = row;i<row + 13;i++){
             usacreport.cellpara cellpara1 = new usacreport.cellpara();
             cellpara1.style = style;
             cellpara1.name = name;
             cellpara1.databasename = dbname;
             cellpara1.flag = flag;
             cellpara1.sdate = sdate;//待处理
             cellpara1.time = time;
             cellpara1.type = 0;
             cellpara1.col = col;//现在考虑col固定
             cellpara1.row = i;
             usacreport.FrmSacReport.vectorpara.addElement(cellpara1);
           }
           return;
         }

         if(style.substring(0,1).compareTo("z") == 0){
           usacreport.cellpara cellpara1 = new usacreport.cellpara();
           cellpara1.style = style;
           cellpara1.name = name;
           cellpara1.databasename = dbname;
           cellpara1.flag = flag;
           cellpara1.sdate = sdate;
           cellpara1.time = time;
           cellpara1.col = col;//现在考虑col固定cellpara1.row = i;
           cellpara1.row = row;
           cellpara1.type = type;
           usacreport.FrmSacReport.vectorpara.addElement(cellpara1);
           return;
         }
       */
   }

   boolean isintab(String tbname) {
      String dbname = "modeldb";
      String tablename = "tab";
      String fieldname = "TNAME";
      String where = "", order = "";

      where = "TNAME = '" + tbname.toUpperCase() + "'";
      int[] icount = new int[1];
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);

      if (returnv > 0) {
         return true;
      }
      else {
         return false;
      }
   }

   int xgd_proc(String type, String name, String tabletime, int begin, int end,
                int nowdate, int selhour, int row, int col) {
      String dbname = "modeldb";
      String tablename = "yc" + tabletime;
      String fieldname = "sdate,time";
      String where = "", order = "";
      int i, j, pos, k;
      int[] icount = new int[1];
      double returnd;
      int returni = 0;
      String str_text = "", str_temp = "", str_temp1 = "", str_temp2 = "";

      usacreport.cellpara cellparaxg = new usacreport.cellpara();
      Vector vecpara = new Vector();
      String str_dayshow = "";
      int day_row = 0, day_col = 0;

      for (i = row; i <= usacreport.FrmSacReport.jBook1.getLastRow(); i++) {
         for (j = col; j <= usacreport.FrmSacReport.jBook1.getLastCol(); j++) {
            try {
//          usacreport.FrmSacReport.jBook1.setActiveCell(i,j);
               str_text = usacreport.FrmSacReport.jBook1.getText(i, j).trim();
               if (str_text.compareTo("") == 0) {
                  continue;
               }
               if (str_text.substring(0, 1).compareTo("@") != 0) {
                  continue;
               }
               if (str_text.substring(1, 4).compareTo("ny3") == 0) { //处理日报表的日期显示
                  str_dayshow = Integer.toString(cal_dt.get(cal_dt.YEAR)) + "-" +
                      Integer.toString(cal_dt.get(cal_dt.MONTH) + 1) + "-" +
                      Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
                  day_row = row;
                  day_col = col;
                  continue;
               }

               str_text = str_text.substring(1, str_text.length());
               if (str_text.substring(0, 2).compareTo("xd") == 0 ||
                   str_text.substring(0, 2).compareTo("xh") == 0) {

                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = j;
                  usac_vec.str_cell = "@," + str_text;
                  veccontent.addElement(usac_vec);

                  str_text = str_text.substring(0, str_text.length());
                  pos = str_text.indexOf(",");
                  name = cellparaxg.name = str_text.substring(pos + 1,
                      str_text.length() - 2);
                  cellparaxg.style = cellparaxg.name;
                  cellparaxg.sdate = nowdate;

                  fieldname = "time,data";

                  order = "";
                  if (str_text.substring(0, 2).compareTo("xd") == 0) {
                     where = "select max(data) from yc" + tabletime +
                         " where name = '" + name + "' and sdate = " +
                         Integer.toString(nowdate);
                  }
                  else {
                     where = "select min(data) from yc" + tabletime +
                         " where name = '" + name + "' and sdate = " +
                         Integer.toString(nowdate);
                  }
                  //System.out.println(where);
                  returnd = usacreport.FrmSacReport.f1jdll.SqlReadRecorder(
                      dbname.getBytes(), where.getBytes(), 0);
//            if(returnd < 0) return 0;
                  String s_data = Double.toString(returnd);
//            int pos_temp = s_data.lastIndexOf(".");
//            s_data = s_data.substring(0,pos_temp+3);
                  usacreport.FrmSacReport.jBook1.setNumber(i, j,
                      Double.parseDouble(s_data));
//            if(returnd < 0){
//              cellparaxg.sdate = -1;
//              vecpara.addElement(cellparaxg);
//              continue;
//            }
                  order = "";
                  fieldname = "time,data";
                  if (str_text.substring(0, 2).compareTo("xd") == 0) {
                     where = "data >= " + s_data + " and name = '" +
                         cellparaxg.name + "' and sdate = " +
                         Integer.toString(nowdate); //太长了
                  }
                  else {
                     where = "data <= " +
                         Float.toString(Float.parseFloat(s_data) + 0.1f) +
                         " and name = '" + cellparaxg.name + "' and sdate = " +
                         Integer.toString(nowdate); //太长了
                  }

                  returni = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
                      getBytes(), tablename.getBytes(), fieldname.getBytes(),
                      where.getBytes(),
                      order.getBytes(), icount, true);
                  String data_com = "";
                  if (returni > 0) {
                     byte[][] buffer = new byte[0][0];
                     buffer = new byte[returni][usacreport.ConstantDef.
                         MAX_COLUMN_LEN];
                     int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                     for (k = 0; k < returni; k += 2) {
                        if (k == 0) {
                           String str1 = new String(buffer[k]);
                           cellparaxg.time = Integer.parseInt(str1.trim());
                           String str2 = new String(buffer[k + 1]);
                           cellparaxg.data = Double.parseDouble(str2.trim());
                        }
                        else {
                           String str2 = new String(buffer[k + 1]);

                           if (str_text.substring(0, 2).compareTo("xd") == 0) {
                              if (cellparaxg.data < Double.parseDouble(str2.
                                  trim())) {
                                 String str1 = new String(buffer[k]);
                                 cellparaxg.time = Integer.parseInt(str1.trim());
                                 cellparaxg.data = Double.parseDouble(str2.trim());
                              }
                           }
                           else {
                              if (cellparaxg.data >=
                                  Double.parseDouble(str2.trim())) {
                                 String str1 = new String(buffer[k]);
                                 cellparaxg.time = Integer.parseInt(str1.trim());
                                 cellparaxg.data = Double.parseDouble(str2.trim());
                              }
                           }
                        }
                     }
//               usacreport.FrmSacReport.jBook1.setActiveCell(day_row,day_col);
                     usacreport.FrmSacReport.jBook1.setText(day_row, day_col,
                         str_dayshow + "  " +
                         Integer.toString(cellparaxg.time / 60) + ":" +
                                       Integer.toString(cellparaxg.time % 60));
                     vecpara.addElement(cellparaxg);
                     continue;
                  }
                  else {
                     cellparaxg.time = -1;
                     vecpara.addElement(cellparaxg); //没有取到基值
                     continue;
                  }
               }
               else {
                  pos = str_text.lastIndexOf(",");
                  str_text = str_text.substring(0, pos);
                  pos = str_text.lastIndexOf(",");
                  String jzname = str_text.substring(0, pos);
                  String psname = str_text.substring(pos + 1, str_text.length());

                  for (k = 0; k < vecpara.size(); k++) {
                     cellparaxg = (usacreport.cellpara) vecpara.get(k);
                     if (cellparaxg.name.compareTo(jzname) == 0) {
                        if (cellparaxg.time == -1) {
                           usacreport.FrmSacReport.jBook1.setNumber(i, j, 0.00);
                           break;
                        }
                        usacreport.cellpara1 usac_vec = new usacreport.
                            cellpara1();
                        usac_vec.row = i;
                        usac_vec.col = j;
                        usac_vec.str_cell = "@," + str_text + ",0";
                        veccontent.addElement(usac_vec);
                        where = "sdate = " + Integer.toString(cellparaxg.sdate) +
                            " and time = " + Integer.toString(cellparaxg.time) +
                            " and name = '" + psname + "'";
                        // System.out.println(where);
                        fieldname = "data";
                        order = "";
                        returni = usacreport.FrmSacReport.f1jdll.readRecorders(
                            dbname.getBytes(), tablename.getBytes(),
                            fieldname.getBytes(), where.getBytes(),
                            order.getBytes(), icount, true);

                        if (returni > 0) {
                           byte[][] buffer = new byte[0][0];
                           buffer = new byte[returni][usacreport.ConstantDef.
                               MAX_COLUMN_LEN];
                           int m_ret = usacreport.FrmSacReport.f1jdll.readdata(
                               buffer);

                           String str1 = new String(buffer[0]);
                           str1 = str1.trim();
//                   int pos_temp = str1.lastIndexOf(".");
//                   str1 = str1.substring(0,pos_temp+3);
                           usacreport.FrmSacReport.jBook1.setNumber(i, j,
                               Double.parseDouble(str1));
                           break;
                        }
                        else {
                           usacreport.FrmSacReport.jBook1.setNumber(i, j, 0.00);
                           break;
                        }
                     }
                  }
               }
            }
            catch (com.f1j.util.F1Exception e1) {
            }
         }
      }
      return 1;
   }

   int m_proc(String type, String name, String tabletime, int begin, int end,
              int nowdate, int selhour, int row, int col) {
      String dbname = "modeldb";
      String tablename = "yc" + tabletime;
      String fieldname = "sdate,time";
      String where = "", order = "";
      int i, j, pos, k;
      int[] icount = new int[1];
      double returnd;

      String str_text = "", str_temp = "", str_temp1 = "", str_temp2 = "";

      usacreport.cellpara cellparaxg = new usacreport.cellpara();
      for (i = row; i <= usacreport.FrmSacReport.jBook1.getLastRow(); i++) {
         for (j = col; j <= usacreport.FrmSacReport.jBook1.getLastCol(); j++) {
            try {
//          usacreport.FrmSacReport.jBook1.setActiveCell(i,j);
               str_text = usacreport.FrmSacReport.jBook1.getText(i, j).trim();
               if (str_text.compareTo("") == 0) {
                  continue;
               }
               if (str_text.substring(0, 1).compareTo("@") != 0) {
                  continue;
               }
               str_text = str_text.substring(1, str_text.length());
               if (str_text.substring(0, 4).compareTo("emax") == 0 ||
                   str_text.substring(0, 4).compareTo("emin") == 0) {
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = j;
                  usac_vec.str_cell = "@," + str_text;
                  veccontent.addElement(usac_vec);
                  str_text = str_text.substring(0, str_text.length());
                  pos = str_text.indexOf(",");
                  name = cellparaxg.name = str_text.substring(pos + 1,
                      str_text.length() - 2);
                  cellparaxg.style = cellparaxg.name;
                  cellparaxg.sdate = nowdate;
                  fieldname = "time,data";
                  order = "";
                  if (str_text.substring(0, 4).compareTo("emax") == 0) {
                     where = "select max(data) from yc" + tabletime +
                         " where name = '" + name +
                         "' and flag <= 50 and mod(time,60)=0 and sdate = " +
                         Integer.toString(nowdate);
                  }
                  else {
                     where = "select min(data) from yc" + tabletime +
                         " where name = '" + name +
                         "' and flag <= 50 and mod(time,60)=0 and sdate = " +
                         Integer.toString(nowdate);
                  }
                  //System.out.println(where);
                  returnd = usacreport.FrmSacReport.f1jdll.SqlReadRecorder(
                      dbname.getBytes(), where.getBytes(), 0);
                  String s_data = Double.toString(returnd);
                  usacreport.FrmSacReport.jBook1.setNumber(i, j,
                      Double.parseDouble(s_data));
               }
            }
            catch (com.f1j.util.F1Exception e1) {
            }
         }
      }
      return 1;
   }

   int xgm_proc(String type, String name, String tabletime, int begin, int end,
                int nowdate, int selhour, int row, int col) {
      String dbname = "modeldb";
      String tablename = "dayyc" + tabletime;
      String fieldname = "sdate,time";
      String where = "", order = "";
      int i, j, pos, k;
      int[] icount = new int[1];
      double returnd;
      int returni = 0;
      String str_text = "", str_temp = "", str_temp1 = "", str_temp2 = "";
      usacreport.cellpara cellparaxg = new usacreport.cellpara();
      Vector vecpara = new Vector();
      String str_dayshow = "";
      int day_row = 0, day_col = 0;

      for (i = row; i <= usacreport.FrmSacReport.jBook1.getLastRow(); i++) {
         for (j = col; j <= usacreport.FrmSacReport.jBook1.getLastCol(); j++) {
            try {
               usacreport.FrmSacReport.jBook1.setActiveCell(i, j);
               str_text = usacreport.FrmSacReport.jBook1.getText();
               if (str_text.compareTo("") == 0) {
                  continue;
               }
               if (str_text.substring(0, 1).compareTo("@") != 0) {
                  continue;
               }
               if (str_text.substring(1, 4).compareTo("ny3") == 0) { //处理日报表的日期显示
                  str_dayshow = Integer.toString(cal_dt.get(cal_dt.YEAR)) + "-" +
                      Integer.toString(cal_dt.get(cal_dt.MONTH) + 1) + "-" +
                      Integer.toString(cal_dt.get(cal_dt.DAY_OF_MONTH));
                  day_row = row;
                  day_col = col;
                  continue;
               }

               str_text = str_text.substring(1, str_text.length());
               if (str_text.substring(0, 2).compareTo("xm") == 0 ||
                   str_text.substring(0, 2).compareTo("xn") == 0) {

                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = i;
                  usac_vec.col = j;
                  usac_vec.str_cell = "@," + str_text;
                  veccontent.addElement(usac_vec);

                  str_text = str_text.substring(0, str_text.length());
                  pos = str_text.indexOf(",");
                  name = cellparaxg.name = str_text.substring(pos + 1,
                      str_text.length() - 2);
                  cellparaxg.style = cellparaxg.name;
                  cellparaxg.sdate = nowdate;

                  fieldname = "sdate,time,data";
                  order = "";
                  if (str_text.substring(0, 2).compareTo("xm") == 0) {
                     where = "select max(data) from yc" + tabletime +
                         " where name = '" + name;
                  }
                  else {
                     where = "select min(data) from yc" + tabletime +
                         " where name = '" + name;
                  }

                  returnd = usacreport.FrmSacReport.f1jdll.SqlReadRecorder(
                      dbname.getBytes(), where.getBytes(), 0);
                  String s_data = Double.toString(returnd);
                  int pos_temp = s_data.lastIndexOf(".");
                  s_data = s_data.substring(0, pos_temp + 3);
                  usacreport.FrmSacReport.jBook1.setNumber(Double.parseDouble(
                      s_data));
//            if(returnd <= 0) {
//              cellparaxg.sdate = -1;
//              vecpara.addElement(cellparaxg);
//              continue;
//            }
                  order = "";
                  fieldname = "sdate,time,data";
                  if (str_text.substring(0, 2).compareTo("xm") == 0) {
                     where = "data >= " + s_data + " and name = '" +
                         cellparaxg.name;
                  }
                  else {
                     where = "data <= " +
                         Float.toString(Float.parseFloat(s_data) + 0.1f) +
                         " and name = '" + cellparaxg.name;
                  }

                  tablename = "yc" + tabletime;
                  returni = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
                      getBytes(), tablename.getBytes(), fieldname.getBytes(),
                      where.getBytes(),
                      order.getBytes(), icount, true);
                  String data_com = "";
                  if (returni >= 0) {
                     byte[][] buffer = new byte[0][0];
                     buffer = new byte[returni][usacreport.ConstantDef.
                         MAX_COLUMN_LEN];
                     int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                     for (k = 0; k < returni; k += 3) {
                        if (k == 0) {
                           String str1 = new String(buffer[k + 2]);
                           cellparaxg.data = Double.parseDouble(str1.trim());
                           String str2 = new String(buffer[k]);
                           cellparaxg.sdate = Integer.parseInt(str2.trim());
                           String str3 = new String(buffer[k + 1]);
                           cellparaxg.time = Integer.parseInt(str3.trim());
                        }
                        else {
                           String str2 = new String(buffer[k + 2]);
                           if (str_text.substring(0, 2).compareTo("xm") == 0) {
                              if (cellparaxg.data < Double.parseDouble(str2.
                                  trim())) {
                                 String str1 = new String(buffer[k]);
                                 String str3 = new String(buffer[k + 1]);
                                 cellparaxg.sdate = Integer.parseInt(str1.trim());
                                 cellparaxg.time = Integer.parseInt(str3.trim());
                                 cellparaxg.data = Double.parseDouble(str2.trim());
                              }
                           }
                           else {
                              if (cellparaxg.data >=
                                  Double.parseDouble(str2.trim())) {
                                 String str1 = new String(buffer[k]);
                                 String str3 = new String(buffer[k + 1]);
                                 cellparaxg.sdate = Integer.parseInt(str1.trim());
                                 cellparaxg.time = Integer.parseInt(str3.trim());
                                 cellparaxg.data = Double.parseDouble(str2.trim());
                              }
                           }
                        }
                     }
                     usacreport.FrmSacReport.jBook1.setActiveCell(day_row,
                         day_col);
                     usacreport.FrmSacReport.jBook1.setCellText(str_dayshow +
                         "  " + Integer.toString(cellparaxg.time / 60) + ":" +
                         Integer.toString(cellparaxg.time % 60));
                     vecpara.addElement(cellparaxg);
                     continue;
                  }
                  else {
                     cellparaxg.time = -1;
                     vecpara.addElement(cellparaxg); //没有取到基值
                     continue;
                  }
               }
               else {
                  pos = str_text.lastIndexOf(",");
                  str_text = str_text.substring(0, pos);
                  pos = str_text.lastIndexOf(",");
                  String jzname = str_text.substring(0, pos);
                  String psname = str_text.substring(pos + 1, str_text.length());

                  for (k = 0; k < vecpara.size(); k++) {
                     cellparaxg = (usacreport.cellpara) vecpara.get(k);
                     if (cellparaxg.name.compareTo(jzname) == 0) {
                        if (cellparaxg.time == -1) {
                           usacreport.FrmSacReport.jBook1.setNumber(0.00);
                           continue;
                        }
                        usacreport.cellpara1 usac_vec = new usacreport.
                            cellpara1();
                        usac_vec.row = i;
                        usac_vec.col = j;
                        usac_vec.str_cell = "@," + str_text + ",0";
                        veccontent.addElement(usac_vec);
                        where = "sdate = " + Integer.toString(cellparaxg.sdate) +
                            " and time = " + Integer.toString(cellparaxg.time) +
                            " and name = '" + psname + "'";
                        //System.out.println(where);
                        fieldname = "data";
                        order = "";
                        returni = usacreport.FrmSacReport.f1jdll.readRecorders(
                            dbname.getBytes(), tablename.getBytes(),
                            fieldname.getBytes(), where.getBytes(),
                            order.getBytes(), icount, true);

                        if (returni > 0) {
                           byte[][] buffer = new byte[0][0];
                           buffer = new byte[returni][usacreport.ConstantDef.
                               MAX_COLUMN_LEN];
                           int m_ret = usacreport.FrmSacReport.f1jdll.readdata(
                               buffer);

                           String str1 = new String(buffer[0]);
                           str1 = str1.trim();
                           int pos_temp = str1.lastIndexOf(".");
                           str1 = str1.substring(0, pos_temp + 3);
                           usacreport.FrmSacReport.jBook1.setNumber(Double.
                               parseDouble(str1));
                           continue;
                        }
                        else {
                           usacreport.FrmSacReport.jBook1.setNumber(0.00);
                           continue;
                        }
                     }
                  }
               }
            }
            catch (com.f1j.util.F1Exception e1) {
            }
         }
      }
      return 1;
   }

   void data_31(String type, String name, String tabletime, int begin, int end,
                int selhour, int row, int col) {
      String dbname = hdb_name;
      String tablename;
      int bdir = 0; //纵向
      if (type.compareTo("31") == 0) { //31为yc ，32为kwh
         tablename = "yc" + tabletime;
      }
      else {
         tablename = "kwh" + tabletime;
      }
      String fieldname = "time,data";
      String where = "", order = "time";

      String str_temp = "";
      try {
         str_temp = usacreport.FrmSacReport.jBook1.getText(row, col + 1).trim();
         if (str_temp.indexOf(",") > 0) {
            str_temp = str_temp.substring(str_temp.indexOf(",") + 1,
                                          str_temp.length());
         }
         int dot_pos = 0;
         dot_pos = str_temp.indexOf(",");
         if (dot_pos > 0) {
            str_temp = str_temp.substring(0, dot_pos);
         }
         if (str_temp.compareTo(name) == 0) {
            bdir = 1; //横向
         }
         else {
            bdir = 0; //纵向
         }
      }
      catch (com.f1j.util.F1Exception edir) {
      }

      int[] icount = new int[1];
      int i = 0, j = 0;
      int pos = 0, firstrow = 0, firstcol = 0;

      firstrow = row;
      firstcol = col;
      if (bdir == 1) {
         pos = col;
      }
      else if (bdir == 0) {
         pos = row;
      }

      if (!isintab(tablename)) {
         return;
      }
      /**    if(!isintab(tablename)){
            try{
              if(bdir == 0){//纵
                for(i=row;i<row+24;i++){
                  usacreport.FrmSacReport.jBook1.setNumber(i,col,0.0000);
                }
              }
              else if(bdir==1){ //横
                  for(i=col;i<col+24;i++){
                  usacreport.FrmSacReport.jBook1.setNumber(row,i,0.0000);
                }
              }
              else{
                usacreport.FrmSacReport.jBook1.setNumber(row,col,0.0000);
              }
            }
            catch(com.f1j.util.F1Exception e){
            }
          }*/

      for (int loop = begin; loop < end; loop++) {
         where = "name = '" + name + "' and sdate = " + Integer.toString(loop)
             + " and flag < 50 and time in (60,120,180,240,300,360,420,480,540,600,660,720,780,840,900,960,1020,1080,1140,1200,1260,1320,1380,1440)";

         int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
             getBytes(), tablename.getBytes(), fieldname.getBytes(),
             where.getBytes(),
             order.getBytes(), icount, true);

         if (returnv <= 0) {
            try {
               if (bdir == 0) { //纵
                  for (i = row; i < row + 24; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = i;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(s_cellvalue);
                  }
               }
               else { //横
                  for (i = col; i < col + 24; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = i;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(s_cellvalue);
                  }
               }
            }
            catch (com.f1j.util.F1Exception e) {
            }
         }
         byte[][] buffer = new byte[0][0];
         buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
         int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
         int time_pos = 60, time_temp = 0;
         String str_1 = "";
         double m_result = 0.0;

         for (j = 0; j < returnv; j += 2) {
            String str1 = new String(buffer[j]); //time
            String str2 = new String(buffer[j + 1]); //data
            str1 = str1.trim();
            str2 = str2.trim(); //.substring(0,6);//锁定6位有效数字(包括小数点)
            m_result = Double.parseDouble(str2);
            if (usacreport.FrmSacReport.babs) {
               if (m_result < 0) {
                  m_result = Math.abs(m_result);
               }
            }

            try {
               str_1 = Integer.toString(time_pos);
               if (Integer.parseInt(str1) < Integer.parseInt(str_1)) {
                  continue; //如果有重复数据
               }
               while (str_1.compareTo(str1) != 0) { //如果中间缺数据
                  if (bdir == 0) {
                     usacreport.FrmSacReport.jBook1.setNumber(pos, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = pos;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(s_cellvalue);
                  }
                  else {
                     usacreport.FrmSacReport.jBook1.setNumber(row, pos, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = pos;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(s_cellvalue);
                  }
                  pos++;
                  time_pos += 60;
                  str_1 = Integer.toString(time_pos);
               }
               if (bdir == 1) {
                  usacreport.FrmSacReport.jBook1.setNumber(row, pos, m_result);
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = row;
                  usac_vec.col = pos;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(s_cellvalue);
               }
               else {
                  usacreport.FrmSacReport.jBook1.setNumber(pos, col, m_result);
                  usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                  usac_vec.row = pos;
                  usac_vec.col = col;
                  usac_vec.str_cell = s_cellvalue;
                  veccontent.addElement(s_cellvalue);
               }

               pos++;
               time_pos += 60;
            }
            catch (com.f1j.util.F1Exception e2) {
            }
         }

         try {
            if (bdir == 1) {
               if (pos < col + 24) {
                  for (i = pos; i < col + 24; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(row, i, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = row;
                     usac_vec.col = i;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(s_cellvalue);
                  }
               }
            }
            else if (bdir == 0) {
               if (pos < row + 24) {
                  for (i = pos; i < row + 24; i++) {
                     usacreport.FrmSacReport.jBook1.setNumber(i, col, 0.0000);
                     usacreport.cellpara1 usac_vec = new usacreport.cellpara1();
                     usac_vec.row = i;
                     usac_vec.col = col;
                     usac_vec.str_cell = s_cellvalue;
                     veccontent.addElement(s_cellvalue);
                  }
               }
            }
         }
         catch (com.f1j.util.F1Exception eei) {
         }

         try {
            usacreport.FrmSacReport.jBook1.recalc();
         }
         catch (com.f1j.util.F1Exception e3) {
         }

         if (bdir == 0) { //纵
            col++;
            pos = firstrow;
         }
         else {
            row++;
            pos = firstcol;
         }
      }
   }



}
