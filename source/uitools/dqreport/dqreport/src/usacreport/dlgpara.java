package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.border.EmptyBorder;
import java.awt.BorderLayout;
import com.f1j.util.*;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgpara extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JComboBox jComtable = new JComboBox();
  private JScrollPane jScrollPane1 = new JScrollPane();
  private JList jListfield = new JList();
  private JButton jButOk = new JButton();
  private JButton jButCancel = new JButton();
  private Vector vectorfield = new Vector();
  private String tbname = "";
  private JLabel jLabel2 = new JLabel();
  private JComboBox jComtem = new JComboBox();
  private Vector vectortem = new Vector();
  private JComboBox jComfw = new JComboBox();
  private JLabel jLabel3 = new JLabel();
  private Vector vecpdb_field = new Vector();
  private Vector vec_yctype = new Vector();
  private Vector vec_yxtype = new Vector();
  private Vector vec_ddtype = new Vector();
  private Vector vec_fwvec = new Vector();
//  private byte [][]bufferfield = new byte[usacreport.ConstantDef.MAX_COLNUM*3][usacreport.ConstantDef.MAX_COLUMN_LEN];
//evaluatemode==5从数据库响应表读取
  public dlgpara(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgpara() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("表名");
    jButOk.setFont(new java.awt.Font("Dialog", 0, 12));
    jButOk.setText("查询");
    jButOk.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButOk_actionPerformed(e);
      }
    });
    jButCancel.setFont(new java.awt.Font("Dialog", 0, 12));
    jButCancel.setToolTipText("");
    jButCancel.setText("取消");
    jButCancel.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButCancel_actionPerformed(e);
      }
    });
    jComtable.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComtable_actionPerformed(e);
      }
    });
    jLabel2.setText("终端");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jComfw.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComfw_actionPerformed(e);
      }
    });
    jLabel3.setText("服务类型");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jComfw.setFont(new java.awt.Font("SansSerif", 0, 12));
    jComtable.setFont(new java.awt.Font("SansSerif", 0, 12));
    jComtem.setFont(new java.awt.Font("SansSerif", 0, 12));
    jListfield.setFont(new java.awt.Font("SansSerif", 0, 12));
      panel1.add(jComfw, new XYConstraints(77, 3, 248, -1));
      panel1.add(jLabel3, new XYConstraints(16, 1, 71, 24));
      panel1.add(jComtable, new XYConstraints(77, 36, 248, -1));
      panel1.add(jComtem, new XYConstraints(77, 68, 248, 24));
      panel1.add(jLabel1, new XYConstraints(39, 37, 41, 28));
      panel1.add(jLabel2, new XYConstraints(39, 63, 30, 28));
      panel1.add(jScrollPane1, new XYConstraints(76, 108, 248, 248));
      panel1.add(jButOk, new XYConstraints(76, 362, 101, 30));
      panel1.add(jButCancel, new XYConstraints(223, 363, 101, 30));
      jScrollPane1.getViewport().add(jListfield);
      this.getContentPane().add(panel1, java.awt.BorderLayout.WEST);
    filltem();
    filltxtfield();
    jComtem.setEnabled(false);
    tablefield();
  }

  void jButCancel_actionPerformed(ActionEvent e) {
    this.hide();
  }

  void jButOk_actionPerformed(ActionEvent e) {
    int i=0,j=0,k=0;
    boolean bfirst = true;
    String dbname = "modeldb";
    String tablename = "";
    String fieldname = "";
    String where = "",order = "";
    Object []object1=null;
    String sql="";
    int step = 0;
    int row=2,col=1;
    Vector vectempdb = new Vector();
    int type_col = -1;
   try {
      usacreport.FrmSacReport.jBook1.initWorkbook();
   }
   catch (OutOfMemoryError ex) {
   }
   catch (F1Exception ex) {
   }
    tablename = (String)jComtable.getSelectedItem();

    if(tbname.length()<0) return;
    if(jComtem.isEnabled()){
      if("全部终端".compareTo(jComtem.getSelectedItem().toString())!=0){
        where = tablename + ".终端序号="+(String)vectortem.get((jComtem.getSelectedIndex()-1)*2);
        order = "终端序号";
      }
    }

    int i_fieldcount = 0;
    String str_1="";
    boolean b_two=false;
    int []icount = new int[1];
    int returnv = 0;
    String str_fd = "";
    object1 = jListfield.getSelectedValues();
    int db_type1 = -1;

    if(tablename.compareTo("遥测参数表")==0){
      for(i=0;i<object1.length;i++){
        if("类型".compareTo(object1[i].toString())==0){
          type_col = i;
          db_type1 = 0;
          break;
        }
      }
    }else if(tablename.compareTo("遥信参数表")==0){
      for(i=0;i<object1.length;i++){
        if("类型".compareTo(object1[i].toString())==0){
          type_col = i;
          db_type1 = 1;
          break;
        }
      }
    }else if(tablename.compareTo("电度参数表")==0){
      for(i=0;i<object1.length;i++){
        if("类型".compareTo(object1[i].toString())==0){
          type_col = i;
          db_type1 = 2;
          break;
        }
      }
    }
    if(object1.length>0){
      if(order.length()>0){
        order = order + "," + (String)object1[0];
      }else{
        order = (String)object1[0];
      }
    }
    fillpdb_field((String)jComtable.getSelectedItem());
    if(vecpdb_field.size() > 0){
      tablename = tablename + ",";//加源表名
//      for(i=0;i<vectorfield.size();i++){
//        if(jListfield.isSelectedIndex(i) == true){
//          i_fieldcount++;
//          object1 = jListfield.getSelectedValues();

          if(object1.length < 0) return;//没选中任何数据
//          System.out.println(Integer.toString(object1.length));
          for(j=0;j<object1.length;j++){
            for(k=0;k<vecpdb_field.size()-1;k+=5){
              str_1 = (String)vecpdb_field.get(k);
              if(str_1.compareTo(object1[j].toString())==0){
                bfirst = false;
                fieldname = fieldname + (String)vecpdb_field.get(k+2)+"."+(String)vecpdb_field.get(k+3)+",";
                if(where.length()<2){
                  where = (String)vecpdb_field.get(k+1) + "." + (String)vecpdb_field.get(k) + " = " + (String)vecpdb_field.get(k+2) + "." + (String)vecpdb_field.get(k+4);
                }else{
                  where = where + " and " + (String)vecpdb_field.get(k+1) + "." + (String)vecpdb_field.get(k) + " = " + (String)vecpdb_field.get(k+2) + "." + (String)vecpdb_field.get(k+4);
                }

                for(int m=0;m<vectempdb.size();m++){
                  if(vectempdb.get(m).equals(vecpdb_field.get(k+2))) { b_two = true;break;}//存在相同
                }
                if(tablename.length()<1){
                  tablename = (String)vecpdb_field.get(k+2) + ",";
                }
                if(!b_two)
                {
                  tablename = tablename + (String)vecpdb_field.get(k+2) + ",";
                  b_two = false;
                }
                break;
              }
            }
          if(bfirst == true){
              fieldname = fieldname + (String)jComtable.getSelectedItem() + "." +object1[j].toString() + ",";
            }
            bfirst = true;
          }
//          step = object1.length;
//          break;
//        }
//      }

      for(i=0;i<object1.length;i++){
        str_fd = str_fd + "field"+Integer.toString(i)+",";
      }
      if(tablename.length()>0) {
        fieldname = fieldname.substring(0,fieldname.lastIndexOf(","));
        tablename = tablename.substring(0,tablename.lastIndexOf(","));
      }
      if(object1.length>0){
        str_fd = str_fd.substring(0,str_fd.length()-1);
      }
      else{
        return;
      }

      sql = "drop view report_temp;";
      returnv = usacreport.FrmSacReport.f1jdll.ExecuteCmd("modeldb".getBytes(),sql.getBytes());
//    sql = sql.substring(sql.indexOf(";"),sql.length());
     // sql = "create view report_temp("+str_fd+") as select "+fieldname + " from "+ tablename  + " where " + where + ";";
      if (where.equalsIgnoreCase(""))
      {
        sql = "create view report_temp("+str_fd+") as select "+fieldname + " from "+ tablename  + ";";
      }else
      {
       String tablename1=tablename.substring(0,5);
        if (tablename1.equalsIgnoreCase("遥测参数表")){
          sql = "create view report_temp("+str_fd+") as select "+fieldname + " from "+ tablename  + " where " + where + " order by 遥测参数表.遥测序号;";
        }else if(tablename1.equalsIgnoreCase("遥信参数表")){
          sql = "create view report_temp("+str_fd+") as select "+fieldname + " from "+ tablename  + " where " + where + " order by 遥信参数表.遥信序号;";
        }else if(tablename1.equalsIgnoreCase("电度参数表")){
          sql = "create view report_temp("+str_fd+") as select "+fieldname + " from "+ tablename  + " where " + where + " order by 电度参数表.电度序号;";
        }else{
          sql = "create view report_temp("+str_fd+") as select "+fieldname + " from "+ tablename  + " where " + where + ";";
        }
      }
      System.out.println(sql);
      returnv = usacreport.FrmSacReport.f1jdll.ExecuteCmd("modeldb".getBytes(),sql.getBytes());
      if(returnv < 1) return;

      dbname = "modeldb";
      tablename = "report_temp";
      fieldname = str_fd;
      where = "";
      order = "";
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
          order.getBytes(),icount,true);

      if(returnv > 0){
        byte [][] buffer = new byte[0][0];
        buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
        try{
          String path = "";
          String filesp = System.getProperty("file.separator");
          path = usacreport.FrmSacReport.s_sacenv + filesp +
                 "reportfile" + filesp + "参数打印" + ".vts";
         // usacreport.FrmSacReport.jBook1.read(path);      //----hongxiang   打开参数打印.vts失败？？？为什么打开这个文件？
          usacreport.FrmSacReport.jBook1.setMaxRow(returnv/object1.length + 10);
          usacreport.FrmSacReport.jBook1.setMaxCol(object1.length);
//          usacreport.FrmSacReport.jBook1.setColWidthAuto(0,usacreport.FrmSacReport.jBook1.getMaxRow(),0,usacreport.FrmSacReport.jBook1.getMaxCol(),true);
          if(jComtem.isEnabled()){
            usacreport.FrmSacReport.jBook1.setText(0,2,(String)jComtem.getSelectedItem() + tbname);
          }else
          {
            usacreport.FrmSacReport.jBook1.setText(0,2,tbname);
          }
          String name=null;
          for(i=0;i<object1.length;i++){
            try{
              name=object1[i].toString();
              usacreport.FrmSacReport.jBook1.setText(row,col++,name);
              }catch(com.f1j.util.F1Exception  e3){}
          }
          String last_type = "";
          for(i = 0;i < returnv - 3;i+=object1.length)
          {
            col=1;row++;
            int mm = 0;
            for(j=0;j<object1.length;j++){
              String str1 = new String(buffer[i+j]);
              str1 = str1.trim();
              if(j==type_col){
                if(last_type.compareTo(str1)!=0){
                  switch(db_type1)
                  {
                    case 0:
                    {
                      for(mm=0;mm<vec_yctype.size();mm+=2){
                        if(str1.compareTo(vec_yctype.get(mm+1).toString())==0){
                          str1 = (String)vec_yctype.get(mm);
                          break;
                        }
                      }
                      break;
                    }
                    case 1:
                    {
                      for(mm=0;mm<vec_yxtype.size();mm+=2){
                        if(str1.compareTo(vec_yxtype.get(mm+1).toString())==0){
                          str1 = (String)vec_yxtype.get(mm);
                          break;
                        }
                      }
                      break;
                    }
                    case 2:
                    {
                      for(mm=0;mm<vec_ddtype.size();mm+=2){
                        if(str1.compareTo(vec_ddtype.get(mm+1).toString())==0){
                          str1 = (String)vec_ddtype.get(mm);
                          break;
                        }
                      }
                      break;
                    }
                  }
                }
              }
              if(str1.compareTo("")==0) str1 = "空";
              usacreport.FrmSacReport.jBook1.setText(row,col++,str1);
            }
          }
          usacreport.FrmSacReport.jBook1.setColWidthAuto(0,0,usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getLastCol(),false);
        }catch(com.f1j.util.F1Exception  e3){
        }//catch(java.io.IOException e4){
     //   }
      }
      else{
//        System.out.println("Failed to read 参数表!!");
      }
    }
    else{
    //本表中没有关联的数据
      tablename = (String)jComtable.getSelectedItem();
      object1 = jListfield.getSelectedValues();
      if (object1.length<=0) {
        javax.swing.JOptionPane.showMessageDialog(this,"请选择要查询的字段");
        return;
      }
      fieldname = "";
      for(i=0;i<object1.length;i++){
        fieldname = fieldname + (String)object1[i] + ",";
      }
      if(fieldname.length()>0){
        fieldname = fieldname.substring(0,fieldname.lastIndexOf(","));
      }

      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
          order.getBytes(),icount,true);

      if(returnv > 0){
        byte [][] buffer = new byte[0][0];
        buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
        try{
          String path = "";
          String filesp = System.getProperty("file.separator");
          path = usacreport.FrmSacReport.s_sacenv + filesp +
                 "reportfile" + filesp + "参数打印" + ".vts";
         // usacreport.FrmSacReport.jBook1.read(path);  //--------------hongxiang
          usacreport.FrmSacReport.jBook1.setMaxRow(returnv/object1.length + 10);
          usacreport.FrmSacReport.jBook1.setMaxCol(object1.length);
//        usacreport.FrmSacReport.jBook1.setColWidthAuto(0,usacreport.FrmSacReport.jBook1.getMaxRow(),0,usacreport.FrmSacReport.jBook1.getMaxCol(),true);
          if(jComtem.isEnabled()){
            usacreport.FrmSacReport.jBook1.setText(0,2,(String)jComtem.getSelectedItem() + tbname);
          }else
          {
            usacreport.FrmSacReport.jBook1.setText(0,2,tbname);
          }

          for(i=0;i<object1.length;i++){
            try{
              usacreport.FrmSacReport.jBook1.setText(row,col++,object1[i].toString());
              }catch(com.f1j.util.F1Exception  e3){}
          }
          for(i = 0;i < returnv - 3;i+=object1.length)
          {
            col=1;row++;
            for(j=0;j<object1.length;j++){
              String str1 = new String(buffer[i+j]);
              str1 = str1.trim();
              if(str1.compareTo("")==0) str1 = "空";
              usacreport.FrmSacReport.jBook1.setText(row,col++,str1);
            }
          }
          usacreport.FrmSacReport.jBook1.setColWidthAuto(0,0,usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getLastCol(),false);
        }catch(com.f1j.util.F1Exception  e3){
        }
      }
      else{
//      System.out.println("Failed to read 参数表!!");
      }
    }
    usacreport.FrmSacReport.jBook1.setDefaultFontSize(270);
    this.hide();
  }

  void tablefield(){
    int i=0;
    String dbname = "modeldb";
    String tablename = "服务类型参数表";
    String fieldname = "服务类型,服务描述";
    String where = "",order = "";

    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(i = 0;i < returnv;i+=2)
      {
        String str1 = new String(buffer[i]);
        str1 = str1.trim();
        vec_fwvec.addElement(str1);
        String str2 = new String(buffer[i+1]);
        str2 = str2.trim();
        jComfw.addItem(str2);
      }
    }
    else{
///     System.out.println("Failed to read 服务类型参数表");
    }
  }

  void jComtable_actionPerformed(ActionEvent e) {
    jListfield.removeAll();
    vectorfield.removeAllElements();
    int i=0;
    String dbname = "modeldb";
    String tablename = "";
    String fieldname = "";
    String where = "",order = "";
    jComtem.setEnabled(false);
    if(jComtable.getItemCount() <=0) return;
    tablename = (String)jComtable.getSelectedItem();
    tbname = tablename = tablename.trim();

    int []icount = new int[1];
    byte []bcount = new byte[1];

    byte [][]bufferfield = new byte[usacreport.ConstantDef.MAX_COLNUM*3][usacreport.ConstantDef.MAX_COLUMN_LEN];
    int m_ret = usacreport.FrmSacReport.f1jdll.getTableInfo(dbname.getBytes(),tablename.getBytes(),bufferfield,
        icount,bcount);

    if(m_ret > 0){
      for(i = 0;i < m_ret;i++)
      {
        String str1 = new String(bufferfield[i*3]);
        str1 = str1.trim();
        if(str1.compareTo("终端序号")==0){
          jComtem.setEnabled(true);
        }
        vectorfield.addElement(str1);
      }
      jListfield.setListData(vectorfield);
    }
    else{
      jListfield.removeAll();
//      System.out.println("Failed to read field!");
    }
    usacreport.FrmSacReport.f1jdll.closeTableInfo();
  }
  void filltem(){
    jComtem.removeAllItems();
    jComtem.addItem("全部终端");
    int i=0;
    String dbname = "modeldb";
    String tablename = "终端参数表";
    String fieldname = "终端序号,描述";
    String where = "",order = "";

    int []icount = new int[1];
    byte []bcount = new byte[1];

    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),
        where.getBytes(),order.getBytes(),icount,true);

    if(returnv > 0){
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i = 0;i < m_ret;i+=2)
      {
        String str1 = new String(buffer[i]);
        str1 = str1.trim();
        String str2 = new String(buffer[i+1]);
        str2 = str2.trim();
        vectortem.addElement(str1);
        vectortem.addElement(str2);
        jComtem.addItem(str2);
      }
    }
    else{
//      System.out.println("Failed to read field!");
    }
  }
  void jComfw_actionPerformed(ActionEvent e) {
    int i=0;
    String dbname = "modeldb";
    String tablename = "pdb_table";
    String fieldname = "tablename";
    String where = "",order = "";

    jComtem.setEnabled(false);
    int []icount = new int[1];
    where = "apptype ='" + (String)vec_fwvec.get(jComfw.getSelectedIndex())+"'";
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
      jComtable.removeAllItems();
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(i = 0;i < returnv;i++)
      {
        String str1 = new String(buffer[i]);
        str1 = str1.trim();
        jComtable.addItem(str1);
      }
    }
    else{
      jComtable.removeAllItems();
//     System.out.println("Failed to read pdb_table!");
    }
  }

  void fillpdb_field(String db){
    vecpdb_field.removeAllElements();
    int i=0;
    String dbname = "modeldb";
    String tablename = "pdb_field";
    String fieldname = "describe,tablename,datasource,srcdatafield,searchedfield";
    String where = "",order = "tablename";

    where = "(evaluatemode = 5 or evaluatemode = 8 or evaluatemode = 15) and tablename = '" + db + "'";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(i = 0;i < returnv;i+=5)
      {
        String str1 = new String(buffer[i]);
        str1 = str1.trim();
        vecpdb_field.addElement(str1);
        String str2 = new String(buffer[i+1]);
        str2 = str2.trim();
        vecpdb_field.addElement(str2);
        String str3 = new String(buffer[i+2]);
        str3 = str3.trim();
        vecpdb_field.addElement(str3);
        String str4 = new String(buffer[i+3]);
        str4 = str4.trim();
        vecpdb_field.addElement(str4);
        String str5 = new String(buffer[i+4]);
        str5 = str5.trim();
        vecpdb_field.addElement(str5);
      }
    }
    else{
//      System.out.println("Failed to read pdb_field");
    }
  }

  void filltxtfield(){
    vec_yctype.addElement("电流");
    vec_yctype.addElement("1");
    vec_yctype.addElement("A相电流");
    vec_yctype.addElement("2");
    vec_yctype.addElement("B相电流");
    vec_yctype.addElement("3");
    vec_yctype.addElement("C相电流");
    vec_yctype.addElement("4");
    vec_yctype.addElement("有功功率");
    vec_yctype.addElement("20");
    vec_yctype.addElement("无功功率");
    vec_yctype.addElement("40");
    vec_yctype.addElement("力率（功率因数）");
    vec_yctype.addElement("61");
    vec_yctype.addElement("频率");
    vec_yctype.addElement("101");
    vec_yctype.addElement("温度");
    vec_yctype.addElement("102");
    vec_yctype.addElement("变压器档位");
    vec_yctype.addElement("107");
    vec_yctype.addElement("其他遥测");
    vec_yctype.addElement("127");
    vec_yctype.addElement("电压");
    vec_yctype.addElement("150");
    vec_yctype.addElement("A电压");
    vec_yctype.addElement("151");
    vec_yctype.addElement("B电压");
    vec_yctype.addElement("152");
    vec_yctype.addElement("C电压");
    vec_yctype.addElement("153");
    vec_yctype.addElement("AB电压");
    vec_yctype.addElement("154");
    vec_yctype.addElement("BC电压");
    vec_yctype.addElement("155");
    vec_yctype.addElement("CA电压");
    vec_yctype.addElement("156");
    vec_yctype.addElement("零序电压");
    vec_yctype.addElement("157");
    vec_yctype.addElement("正序电压");
    vec_yctype.addElement("158");
    vec_yctype.addElement("负序电压");
    vec_yctype.addElement("159");

    vec_yxtype.addElement("开关");
    vec_yxtype.addElement("300");
    vec_yxtype.addElement("小车开关");
    vec_yxtype.addElement("301");
    vec_yxtype.addElement("状态");
    vec_yxtype.addElement("400");
    vec_yxtype.addElement("刀闸");
    vec_yxtype.addElement("500");
    vec_yxtype.addElement("保护");
    vec_yxtype.addElement("600");
    vec_yxtype.addElement("事故总");
    vec_yxtype.addElement("601");
    vec_yxtype.addElement("预告信号");
    vec_yxtype.addElement("602");

    vec_ddtype.addElement("有功电度");
    vec_ddtype.addElement("200");
  }
}
