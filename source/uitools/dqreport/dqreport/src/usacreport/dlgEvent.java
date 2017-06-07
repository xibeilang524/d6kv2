package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Calendar;
import java.util.Vector;
import java.awt.BorderLayout;
import java.awt.Dimension;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgEvent extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JLabel jLabel2 = new JLabel();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JLabel jLabel5 = new JLabel();
  private JLabel jLabel6 = new JLabel();
  private JLabel jLabel7 = new JLabel();
  private JComboBox jComyearb = new JComboBox();
  private JComboBox jCommonb = new JComboBox();
  private JComboBox jComdayb = new JComboBox();
  private JComboBox jComhourb = new JComboBox();
  private JComboBox jComfz = new JComboBox();
  private JComboBox jComyeare = new JComboBox();
  private JComboBox jCommone = new JComboBox();
  private JComboBox jComdaye = new JComboBox();
  private JComboBox jComhoure = new JComboBox();
  private JComboBox jComfl = new JComboBox();
  private JComboBox jComlx = new JComboBox();
  private JComboBox jComzsxx = new JComboBox();
  private JButton jButOK = new JButton();
  private JButton jButCan = new JButton();
  String dbname = "modeldb";
  String tablename = "",tbname = "";
  String fieldname = "";
  String where = "",order = "";
  private JComboBox jComminb = new JComboBox();
  private JComboBox jCommine = new JComboBox();
  private JLabel jLabel8 = new JLabel();
  private JLabel jLabel10 = new JLabel();
  private JButton jButtest = new JButton();
  private JLabel jLabel14 = new JLabel();
  private JLabel jLabel15 = new JLabel();
  private JLabel jLabel16 = new JLabel();
  private JLabel jLabel17 = new JLabel();
  private JLabel jLabel18 = new JLabel();
  private JLabel jLabel19 = new JLabel();
  private JLabel jLabel110 = new JLabel();
  private JLabel jLabel111 = new JLabel();
  private JLabel jLabel9 = new JLabel();
  private JComboBox jComfz1 = new JComboBox();
  private JScrollPane jScrollPane1 = new JScrollPane();
  private JList jListdm = new JList();
  private Vector vecid = new Vector();
  private Vector vecdec = new Vector();
  private Vector veceventzsxx = new Vector();
  private Calendar calbegin = Calendar.getInstance();
  private Calendar calend = Calendar.getInstance();
  private FillThread fthread = new FillThread();
  private Vector datavec = new Vector();
  public static boolean threadisrun = false;
  private static boolean searchend = true;

  public dlgEvent(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgEvent() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    calbegin.set(Calendar.ZONE_OFFSET, 28800000);
    calend.set(Calendar.ZONE_OFFSET, 28800000);

      panel1.setLayout(xYLayout1);
      panel1.setBorder(new EmptyBorder(5, 5, 15, 15));
      jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel1.setText("起始时间");
      jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel2.setText("终止时间");
      jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel3.setText("分组");
      jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel4.setText("事项分类");
      jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel5.setText("事项类型");
      jLabel6.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel6.setText("站所线系");
      jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel7.setText("对象");
      jButOK.setFont(new java.awt.Font("Dialog", 0, 12));
      jButOK.setText("查询");
      jButOK.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButOK_actionPerformed(e);
         }
      });
      jButCan.setFont(new java.awt.Font("Dialog", 0, 12));
      jButCan.setText("取消");
      jButCan.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButCan_actionPerformed(e);
         }
      });
      jCommonb.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jCommonb_actionPerformed(e);
         }
      });
      jCommone.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jCommone_actionPerformed(e);
         }
      });
      jComyeare.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComyeare_actionPerformed(e);
         }
      });
      jComyearb.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComyearb_actionPerformed(e);
         }
      });
      jComzsxx.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComzsxx_actionPerformed(e);
         }
      });
      jComfl.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComfl_actionPerformed(e);
         }
      });
      jLabel8.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel8.setText("年");
      jLabel10.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel10.setToolTipText("");
      jLabel10.setText("月");
      jButtest.setEnabled(false);
      jButtest.setFont(new java.awt.Font("Dialog", 0, 12));
      jButtest.setText("测试");
      jButtest.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButtest_actionPerformed(e);
         }
      });
      jLabel14.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel14.setText("年");
      jLabel15.setText("月");
      jLabel15.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel15.setToolTipText("");
      jLabel16.setText("日");
      jLabel16.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel16.setToolTipText("");
      jLabel17.setText("日");
      jLabel17.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel17.setToolTipText("");
      jLabel18.setText("时");
      jLabel18.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel18.setToolTipText("");
      jLabel19.setText("分");
      jLabel19.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel19.setToolTipText("");
      jLabel110.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel110.setToolTipText("");
      jLabel110.setText("分");
      jLabel111.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel111.setToolTipText("");
      jLabel111.setText("时");
      jLabel9.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel9.setText("表名");
      jComfz1.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComfz1_actionPerformed(e);
         }
      });
      jComyearb.setFont(new java.awt.Font("Dialog", 0, 12));
      jComyeare.setFont(new java.awt.Font("Dialog", 0, 12));
      jComdaye.setFont(new java.awt.Font("Dialog", 0, 12));
      jComdayb.setFont(new java.awt.Font("Dialog", 0, 12));
      jCommonb.setFont(new java.awt.Font("Dialog", 0, 12));
      jCommone.setFont(new java.awt.Font("Dialog", 0, 12));
      jComhoure.setFont(new java.awt.Font("Dialog", 0, 12));
      jComhourb.setFont(new java.awt.Font("Dialog", 0, 12));
      jComminb.setFont(new java.awt.Font("Dialog", 0, 12));
      jCommine.setFont(new java.awt.Font("Dialog", 0, 12));
      jComfl.setFont(new java.awt.Font("Dialog", 0, 12));
      jComzsxx.setFont(new java.awt.Font("Dialog", 0, 12));
      jComlx.setFont(new java.awt.Font("Dialog", 0, 12));
      jComlx.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComlx_actionPerformed(e);
         }
      });
      jComfz.setFont(new java.awt.Font("Dialog", 0, 12));
      jComfz.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
          //  jComfz_actionPerformed(e);
         }
      });
      jComfz1.setFont(new java.awt.Font("Dialog", 0, 12));
      jListdm.setEnabled(false);
      jListdm.setFont(new java.awt.Font("Dialog", 0, 12));
    getContentPane().add(panel1);
      panel1.add(jLabel1,  new XYConstraints(11, 17, 60, 24));
    panel1.add(jLabel2,   new XYConstraints(11, 64, 60, 24));
    panel1.add(jComyearb,   new XYConstraints(86, 20, 63, -1));
    panel1.add(jComyeare,   new XYConstraints(86, 66, 63, -1));
    panel1.add(jComdaye,          new XYConstraints(234, 66, 45, 27));
    panel1.add(jComdayb,        new XYConstraints(234, 20, 45, 27));
    panel1.add(jCommonb,   new XYConstraints(171, 20, 45, 27));
    panel1.add(jCommone,      new XYConstraints(171, 66, 45, 27));
    panel1.add(jLabel15,    new XYConstraints(220, 66, -1, 27));
    panel1.add(jLabel10,    new XYConstraints(220, 20, -1, 27));
    panel1.add(jLabel16,    new XYConstraints(282, 20, -1, 27));
    panel1.add(jLabel17,     new XYConstraints(282, 66, -1, 27));
    panel1.add(jComhoure,     new XYConstraints(298, 66, 45, 27));
    panel1.add(jComhourb,     new XYConstraints(298, 20, 45, 27));
    panel1.add(jLabel18,     new XYConstraints(346, 20, -1, 27));
    panel1.add(jLabel111,     new XYConstraints(346, 66, -1, 27));
    panel1.add(jComminb,     new XYConstraints(362, 20, 45, 27));
    panel1.add(jCommine,     new XYConstraints(362, 66, 45, 27));
    panel1.add(jLabel19,   new XYConstraints(408, 20, -1, 25));
    panel1.add(jLabel110,   new XYConstraints(408, 66, -1, 25));
    panel1.add(jComfl,         new XYConstraints(86, 197, 316, -1));
    panel1.add(jComzsxx,         new XYConstraints(86, 288, 316, -1));
    panel1.add(jComlx,         new XYConstraints(86, 243, 316, -1));
    panel1.add(jComfz,         new XYConstraints(86, 152, 316, -1));
    panel1.add(jLabel3,   new XYConstraints(11, 148, 60, 24));
    panel1.add(jLabel4,   new XYConstraints(11, 195, -1, 24));
    panel1.add(jLabel5,   new XYConstraints(11, 241, 60, 24));
    panel1.add(jLabel6,   new XYConstraints(11, 288, 60, 24));
    panel1.add(jComfz1,       new XYConstraints(86, 106, 316, -1));
    panel1.add(jLabel9,   new XYConstraints(11, 106, 60, 24));
    panel1.add(jScrollPane1,     new XYConstraints(86, 337, 316, 178));
    panel1.add(jLabel7, new XYConstraints(11, 412, 60, 24));
      panel1.add(jLabel14,   new XYConstraints(151, 66, 17, 27));
    panel1.add(jLabel8,   new XYConstraints(151, 20, 17, 27));
   // panel1.add(jButtest,   new XYConstraints(289, 546, 64, 11));
    jScrollPane1.getViewport().add(jListdm, null);
      panel1.add(jButCan, new XYConstraints(288, 534, 103, 30));
      panel1.add(jButOK, new XYConstraints(107, 534, 94, 30));
      int i;
    Calendar calTmp=Calendar.getInstance();
    int tmpYear=calTmp.get(Calendar.YEAR);
    for(i=2005;i<=2020;i++)    //hongxiang  查询事项时，时间选择错误
    {
      jComyearb.addItem(Integer.toString(i));
      jComyeare.addItem(Integer.toString(i));
    }
    for(i=1;i<13;i++)
    {
      jCommonb.addItem(Integer.toString(i));
      jCommone.addItem(Integer.toString(i));
    }
    for(i=0;i<25;i++)
    {
      jComhourb.addItem(Integer.toString(i));
      jComhoure.addItem(Integer.toString(i));
    }
    for(i=0;i<61;i++)
    {
      jComminb.addItem(Integer.toString(i));
      jCommine.addItem(Integer.toString(i));
    }

    dayfill(1,0);
    Calendar cal1 = Calendar.getInstance();
    cal1.set(Calendar.ZONE_OFFSET, 28800000);

    try{
      jComyearb.setSelectedIndex(cal1.get(cal1.YEAR)-2005);
      jComyeare.setSelectedIndex(cal1.get(cal1.YEAR)-2005);
      jCommonb.setSelectedIndex(cal1.get(cal1.MONTH));
      jCommone.setSelectedIndex(cal1.get(cal1.MONTH));
      jComdayb.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      jComdaye.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      jComhoure.setSelectedIndex(cal1.get(cal1.HOUR_OF_DAY));
    }catch(java.lang.IllegalArgumentException elegal){}
    fillfz();

    jComfl.addItem("所有类型");
    jComfl.addItem("空事项类");
    jComfl.addItem("系统中的设备及程序运行事件");
    jComfl.addItem("SCADA事件");
    jComfl.addItem("综合自动化保护事件");
    jComfl.addItem("DAS应用事件");
    jComfl.addItem("DA应用事件");
    jComfl.addItem("DA研究态应用事件");
    jComfl.addItem("EMS应用事件");
    jComfl.addItem("操作事件");
    jComfl.addItem("通知事件");
    jComfl.addItem("WEB应用事件");
    jComfl.addItem("维护类应用事件");

    jComfz1.addItem("全部");
    jComfz1.addItem("电力事项");
    jComfz1.addItem("操作事项");
    fillzsxx();
    jComzsxx.setEnabled(false);
    fthread.start();
  }

  class FillThread extends Thread{
    public void run(){
      try{
        while(true){
          int row_inthread = 4;
          threadisrun = true;
          while(datavec.size()!=0 || !searchend){
            int jishu = 0;
            int i=0,j=0;
            for(j=0;j<datavec.size();j++){
              byte [][]buffertemp = (byte[][])datavec.get(j);
              datavec.removeElementAt(0);
              for(i = 0;i < buffertemp.length;i+=4)
              {
//                if(jishu % 500 == 0) {
//                  System.out.println(Integer.toString(jishu));
//                }
//                jishu++;
                try{
                  String str1 = new String(buffertemp[i]);
                  str1 = str1.trim();
                  if(str1.length() == 8){
                    str1 = str1.substring(0,4) + "年" + str1.substring(4,6)+"月"+str1.substring(6,8)+"日";
                  }
                  usacreport.FrmSacReport.jBook1.setText(row_inthread,1,str1);
                  String str2 = new String(buffertemp[i+1]);
                  str2 = str2.trim();
                  String strrrrrr = str2;
                  int str_length = str2.length();
                  if(str2.length()!=9 && str2.length()<10){
                    for(int nn=0;nn<9-str_length;nn++){
                      str2 = "0" + str2;
                    }
                  }

                  int time_temp = 0;
                  str2 = str2.substring(0,2) + "时" + str2.substring(2,4)+"分"+str2.substring(4,6)+"秒"+str2.substring(6,9)+"毫秒";

                  usacreport.FrmSacReport.jBook1.setText(row_inthread,2,str2);
                  String str3 = new String(buffertemp[i+2]);
                  str3 = str3.trim();
                  usacreport.FrmSacReport.jBook1.setText(row_inthread,3,idtodec(str3));
                  String str5 = new String(buffertemp[i+3]);
                  str5 = str5.trim();
                  usacreport.FrmSacReport.jBook1.setText(row_inthread,4,str5);
                  row_inthread ++;
                }catch(com.f1j.util.F1Exception  e5){
                }
              }
          }
          Thread.sleep(500);
        }
        threadisrun = false;
        Thread.sleep(3000);
        }
      }
      catch(java.lang.InterruptedException ee){}
    }
  }

  void dayfill(int mon,int id)
  {
    int i;
    if(id == 0){
      jComdayb.removeAllItems();
    }else{
      jComdaye.removeAllItems();
    }

    switch(mon)
    {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
      {
        if(id == 0){
          for(i=1;i<32;i++)
          {
              jComdayb.addItem(Integer.toString(i));
          }
        }else{
          for(i=1;i<32;i++)
          {
              jComdaye.addItem(Integer.toString(i));
          }
        }
        break;
      }
      case 4:
      case 6:
      case 9:
      case 11:
      {
        if(id == 0){
          for(i=1;i<31;i++)
          {
            jComdayb.addItem(Integer.toString(i));
          }
        }else{
          for(i=1;i<31;i++)
          {
              jComdaye.addItem(Integer.toString(i));
          }
        }
        break;
      }
      case 2:
      {
        if(id == 0){
          if((jComyearb.getSelectedIndex()+2005)%4!=0)
          {
            for(i=1;i<29;i++)
              {
                  jComdayb.addItem(Integer.toString(i));
              }
          }
          else  if((jComyearb.getSelectedIndex()+2005)%100!=0)
          {
              for(i=1;i<30;i++)
              {
                  jComdayb.addItem(Integer.toString(i));
              }
          }
          else  if((jComyearb.getSelectedIndex()+2005)%400!=0)
          {
              for(i=1;i<29;i++)
              {
                  jComdayb.addItem(Integer.toString(i));
              }
          }
          else
          {
              for(i=1;i<30;i++)
              {
                  jComdayb.addItem(Integer.toString(i));
              }
          }
        }else{
          if((jComyeare.getSelectedIndex()+2005)%4!=0)
          {
            for(i=1;i<29;i++)
              {
                  jComdaye.addItem(Integer.toString(i));
              }
          }
          else  if((jComyeare.getSelectedIndex()+2005)%100!=0)
          {
              for(i=1;i<30;i++)
              {
                  jComdaye.addItem(Integer.toString(i));
              }
          }
          else  if((jComyeare.getSelectedIndex()+2005)%400!=0)
          {
              for(i=1;i<29;i++)
              {
                  jComdaye.addItem(Integer.toString(i));
              }
          }
          else
          {
              for(i=1;i<30;i++)
              {
                  jComdaye.addItem(Integer.toString(i));
              }
          }
        }
      }
    }

//    fill_cal();
  }

  void jButCan_actionPerformed(ActionEvent e) {
    dispose();
  }

  void jCommonb_actionPerformed(ActionEvent e) {
    dayfill(jCommonb.getSelectedIndex()+1,0);
  }

  void jCommone_actionPerformed(ActionEvent e) {
    dayfill(jCommone.getSelectedIndex()+1,1);
  }

  void jComyeare_actionPerformed(ActionEvent e) {
    dayfill(jCommone.getSelectedIndex()+1,1);
  }

  void jComyearb_actionPerformed(ActionEvent e) {
    dayfill(jCommonb.getSelectedIndex()+1,0);
  }

  void fillfz()
  {
    int j;
    tablename = "组参数表";
    fieldname = "项目名";
    where = "";
    order = "";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
//    System.out.println(where);
    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j++)
      {
        String str1 = new String(buffer[j]);
        str1 = str1.trim();
        jComfz.addItem(str1);
      }
      try{
        jComfz.setSelectedIndex(0);
      }
      catch(java.lang.IllegalArgumentException jCom){
      }
    }
  }

  void fillzsxx()
  {
    int j;
//    jComzsxx.setEnabled(true);
    jComzsxx.removeAllItems();
    jComzsxx.addItem("全部");
    vecid.removeAllElements();
    vecdec.removeAllElements();
    vecid.addElement("全部");
    vecdec.addElement("全部");

    fieldname = "代码,描述";
    tablename = "站所线系参数表";
    where = "";
    order = "";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j+=2)
      {
        String str1 = new String(buffer[j]);
        str1 = str1.trim();
        veceventzsxx.addElement(str1);
        String str2 = new String(buffer[j+1]);
        str2 = str2.trim();
        jComzsxx.addItem(str2);
      }
    }
    jListdm.setListData(vecdec);
  }

  void filljdqx()
  {
    int j;
    jComzsxx.setEnabled(false);
    jListdm.removeAll();
    vecid.removeAllElements();
    vecdec.removeAllElements();
    vecid.addElement("全部");
    vecdec.addElement("全部");
    tablename = "节点权限参数表";
    fieldname = "主机名";
    where = "";
    order = "";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j++)
      {
        String str1 = new String(buffer[j]);
        str1 = str1.trim();
        vecid.addElement(str1);
      }
      jListdm.setListData(vecid);
    }
    else{
      jListdm.setListData(vecid);
    }
    jListdm.setSelectedIndex(0);
  }

  void jComzsxx_actionPerformed(ActionEvent e) {
    filldm();
  }

  void jComfl_actionPerformed(ActionEvent e) {
    int j;
    jComlx.removeAllItems();
    jComlx.addItem("全部");
    if(jComfl.getSelectedIndex() == 0) return;

    where = "";
    where = "分类号 = " + Integer.toString(jComfl.getSelectedIndex()-1);
    tablename = "事项类型表";
    fieldname = "类型名";
    order = "";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

//    System.out.println(where);
    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j++)
      {
        String str1 = new String(buffer[j]);
        str1 = str1.trim();
        jComlx.addItem(str1);
      }
    }

    if(jComfz1.getSelectedIndex()==1){//电力历史事项
      if(jComfl.getSelectedIndex()==3){
        jComzsxx.setEnabled(true);
        jListdm.setEnabled(true);
//        fillzsxx();
        jComzsxx.setSelectedIndex(0);
      }
      else{
        jComzsxx.setEnabled(false);
        jListdm.setEnabled(false);
      }
    }
    else if(jComfz1.getSelectedIndex()==2){//操作历史事项
      jComzsxx.setEnabled(false);
      jListdm.setEnabled(true);
      filljdqx();
      jListdm.setSelectedIndex(0);
    }
    else{
      jComzsxx.setEnabled(false);
      jListdm.setEnabled(false);
    }
  }

  void jButOK_actionPerformed(ActionEvent e) {
    int i=0,cur_row=4,j=0,sel_max = 0;
    String s_tbname="",dateb ="",datee="",timeb="",timee="";
    boolean b_first = true;
    searchend= false;
    datavec.removeAllElements();
    if(jComyearb.getSelectedIndex() > jComyeare.getSelectedIndex()) {
      JOptionPane.showMessageDialog(this,"起始年份<终止年份");
      return;
    }
    if(jCommonb.getSelectedIndex() > jCommone.getSelectedIndex() && jComyearb.getSelectedIndex()==jComyeare.getSelectedIndex()){
      JOptionPane.showMessageDialog(this,"起始月份<终止年份");
      return;
    }

    try{
      usacreport.FrmSacReport.jBook1.initWorkbook();
      String filesp = System.getProperty("file.separator");
      String path = "";

      path = usacreport.FrmSacReport.s_sacenv + filesp +
             "reportfile" + filesp + "历史事项.vts";

     // usacreport.FrmSacReport.jBook1.read(path);
      usacreport.FrmSacReport.jBook1.setText(1,1,"分组: "+(String)jComfz.getSelectedItem());
      usacreport.FrmSacReport.jBook1.setText(1,3,"分类: "+(String)jComfl.getSelectedItem());
      usacreport.FrmSacReport.jBook1.setText(1,4,"类型: "+(String)jComlx.getSelectedItem());
    }catch(com.f1j.util.F1Exception  e3){
    }//catch(java.io.IOException e4){
   // }

    String str_lx="";
    int returnv=0;
    double returnd =0.0d;
    int []icount = new int[1];
    if(jComlx.getSelectedIndex() != 0){
      order = "";
      fieldname = "类型号";
      tablename = "事项类型表";
      where = "类型名 = '" + (String)jComlx.getSelectedItem() + "'";
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
      if(returnv > 0){
       byte [][] buffer = new byte[0][0];
        buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

        for(i = 0;i < returnv;i++)
        {
          String str1 = new String(buffer[i]);
          str_lx = str1.trim();
          break;
        }
      }
    }
    fieldname = "年月日,时分秒毫秒,对象名,文字";//分组号,分类号,类型,
    order = "年月日,时分秒毫秒";

    where = "分组号 = 1 ";
    Object []objsel = jListdm.getSelectedValues();
    int where_count = 0;
//    if(objsel.length > 8) {JOptionPane.showMessageDialog(this,"请选择小于8个代码"); return ;}
    if(jListdm.isSelectedIndex(0)==false && jListdm.isEnabled()){
      where = "(";
      for(i=0;i<vecid.size();i++){
        if(jListdm.isSelectedIndex(i)){
          where_count ++;
          if(objsel.length == where_count){
            where = where + "成员0 = '" + (String)vecid.get(i) + "') ";
            break;
          }
          else{
            where = where + "成员0 = '" + (String)vecid.get(i) + "' or ";
          }
        }
      }
    }
    else{
      if(jComzsxx.getSelectedIndex()!=0 && jComzsxx.isEnabled()==true){
        if(where.length()>0){
          where = where + " and 对象名 ='" + (String)veceventzsxx.get(jComzsxx.getSelectedIndex()-1) + "' ";
        }
        else{
          where = "对象名 ='" + (String)veceventzsxx.get(jComzsxx.getSelectedIndex()-1) + "' ";
        }
      }
    }

    if(where.length()>4000){
      JOptionPane.showMessageDialog(this,"查询条件长度>4K字节");
      this.hide();
      return;
    }
    if(jComfl.getSelectedIndex() != 0){
      if(where.length()>0){
        where = where + " and 分类号 = " + Integer.toString(jComfl.getSelectedIndex()-1);
      }
      else{
        where = where + "分类号 = " + Integer.toString(jComfl.getSelectedIndex()-1);
      }
    }
    if(str_lx != ""){
      if(where.length()>0){
        where = where + " and 类型 = " + str_lx;
      }
      else{
        where = where + "类型 = " + str_lx;
      }
    }

    int where_timetype = 0;//0,不跨月,不跨年,不跨天  2,只跨天  3,跨月不跨年  3,跨年
    int table_num = 1;//需要读取表的数目
    int day_num = 0;//1天 2天  or >3天

    int byear=0,eyear=0,bmon=0,emon=0,bday=0,eday=0,bhour=0,ehour=0,bmin=0,emin=0;
    byear = jComyearb.getSelectedIndex();
    eyear = jComyeare.getSelectedIndex();
    bmon = jCommonb.getSelectedIndex();
    emon = jCommone.getSelectedIndex();
    bday = jComdayb.getSelectedIndex()+1;
    eday = jComdaye.getSelectedIndex()+1;
    bhour = jComhourb.getSelectedIndex();
    ehour = jComhoure.getSelectedIndex();
    bmin = jComminb.getSelectedIndex();
    emin = jCommine.getSelectedIndex();

    if(eyear>byear){
      table_num = (12 - bmon) + emon + 1 + (eyear-byear-1)*12;
      where_timetype = 3;
    }
    else if(emon>bmon){
      table_num = emon-bmon+1;
      where_timetype = 2;
    }
    else if(eday>bday){
      where_timetype = 1;
      table_num=1;
    }
    else{
      where_timetype = 0;
      table_num = 1;
    }
    dateb = Integer.toString((byear+2005)*10000 + (bmon+1)*100 + (bday));
    datee = Integer.toString((eyear+2005)*10000 + (emon+1)*100 + (eday));
    String wheresel = where;
    for(j=0;j<table_num;j++){
      wheresel = where;
      timeb = Integer.toString(bhour*10000000 + jComminb.getSelectedIndex()*100000);
      timee = Integer.toString(ehour*10000000 + jCommine.getSelectedIndex()*100000);
      if(j==0){
        calbegin.set(byear+2005,bmon,bday);
      }
      else{
        calbegin.set(byear+2005,bmon,1);
      }
      calend.set(eyear+2005,emon,eday);
      day_num = (int)(calend.getTime().getTime()/86400000-calbegin.getTime().getTime()/86400000)+1;
      if(day_num >2) day_num = 3;
      if(wheresel.length()>0){
        wheresel = where + " and ";
      }
      switch(day_num){
        case 1:
        {
          wheresel = wheresel + "年月日 = " + datee + " and 时分秒毫秒 <= " + timee;
          break;
        }
        case 2:
        {
          wheresel = wheresel + "((年月日 = " + dateb +" and 时分秒毫秒 >= " + timeb
                + " and 时分秒毫秒 <= 240000000" + ") or " + "(年月日 = " + datee
                + " and 时分秒毫秒 >= 0" + " and 时分秒毫秒 <= " + timee + "))";
          break;
        }
        case 3:
        {
          wheresel = wheresel + "((年月日 = " + dateb + " and 时分秒毫秒 >= " + timeb
                + " and 时分秒毫秒 <= 240000000"
                + ") or (年月日 > " + dateb + " and 年月日 < " + datee + ") or "
                + "(年月日 = " + datee
                + " and 时分秒毫秒 >= 0" + " and 时分秒毫秒 <= " + timee + "))";
          break;
        }
      }

      if(jComfz1.getSelectedIndex()==1){//电力事项
        tablename = "电力历史事项表" + Integer.toString((byear+2005)*100+bmon+1);
      }else if(jComfz1.getSelectedIndex() == 2){
        tablename = "操作历史事项表" + Integer.toString((byear+2005)*100+bmon+1);
      }else{
        tablename = "操作历史事项表" + Integer.toString((byear+2005)*100+bmon+1);
          returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),wheresel.getBytes(),
              order.getBytes(),icount,true);

          sel_max = sel_max + returnv/4;
          System.out.println("查询到" + Integer.toString(sel_max) + "条记录!");
          try{
            if(sel_max > 60000){
              JOptionPane.showMessageDialog(this,"查询记录数目超出报表所支持的最大值");
              this.hide();
              return;
            }
            usacreport.FrmSacReport.jBook1.setMaxRow(sel_max + 10);
          }catch(com.f1j.util.F1Exception  e6){
          }

          if(returnv > 0){
                byte [][] buffer = new byte[0][0];
                buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
                int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                int jishu = 0;
                datavec.addElement(buffer);
          }
          tablename = "电力历史事项表" + Integer.toString((byear+2005)*100+bmon+1);
      }

        int returnv1 = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),wheresel.getBytes(),
            order.getBytes(),icount,true);
        sel_max = sel_max + (int)returnv1/4;
        System.out.println("查询到" + Double.toString(sel_max) + "条记录!");
        try{
          if(sel_max > 60000){
            JOptionPane.showMessageDialog(this,"查询记录数目超出报表所支持的最大值");
            this.hide();
            return;
          }
          usacreport.FrmSacReport.jBook1.setMaxRow((int)sel_max + 10);
        }catch(com.f1j.util.F1Exception  e6){
        }
        if(returnv1 > 0){
          byte [][] buffer = new byte[0][0];
          buffer = new byte[returnv1][usacreport.ConstantDef.MAX_COLUMN_LEN];
          int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
          int jishu = 0;
          datavec.addElement(buffer);
      }

      if(bmon == 11){
        byear ++;
        bmon = 0;
        bday = 0;
      }else{
        bday = 0;
        bmon++;
      }
    }
    try{
      usacreport.FrmSacReport.jBook1.setText(2,4,"共检索到: "+Double.toString(sel_max) +"条记录");
//      usacreport.FrmSacReport.jBook1.setColWidthAuto(0,0,usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getLastCol(),false);;
    }catch(com.f1j.util.F1Exception eauto){
    }
    searchend = true;
    usacreport.FrmSacReport.jBook1.setDefaultFontSize(300);
    this.hide();
  }

  void jButtest_actionPerformed(ActionEvent e) {
/*    int j;
    tablename = "";
    fieldname = "max(data)";
    where = "select max(data) from yc200301";
    order = "";
    int []icount = new int[1];

    double returnv = usacreport.FrmSacReport.f1jdll.SqlReadRecorder(dbname.getBytes(),where.getBytes(),0);

    if(returnv > 0){
      returnv = returnv;
    }
    else{
      System.out.println("Failed to read 测试程序");
    }
    int sdate = 12113;
    long s_time = (long)sdate * 86400000;
    java.util.Date s_date = null;
    s_date.setTime(s_time);
    java.util.Calendar c_date = java.util.Calendar.getInstance();
    c_date.setTime(s_date);
    int temp = c_date.get(java.util.Calendar.YEAR);
    temp = c_date.get(java.util.Calendar.YEAR);
    temp = c_date.get(java.util.Calendar.MONTH);
    temp = c_date.get(java.util.Calendar.DAY_OF_MONTH);*/
    try{
      usacreport.FrmSacReport.jBook1.setText(2,3,"2 行 3列");
//      usacreport.FrmSacReport.jBook1.deleteRange(0,0,usacreport.FrmSacReport.jBook1.getSelEndRow(),usacreport.FrmSacReport.jBook1.getSelEndCol(),usacreport.FrmSacReport.jBook1.eShiftRows);
    }catch(com.f1j.util.F1Exception eeee){}
  }

  void jComfz1_actionPerformed(ActionEvent e) {
    switch(jComfz1.getSelectedIndex())
    {
      case 0:
      {
        jComzsxx.setEnabled(false);
        jListdm.setEnabled(false);
        break;
      }
      case 1:
      {
        if(jComfl.getSelectedIndex() == 3){
          jComzsxx.setEnabled(true);
          jListdm.setEnabled(true);
          filldm();
        }
        else{
          jComzsxx.setEnabled(false);
          jListdm.setEnabled(false);
        }
        break;
      }
      case 2:
      {
        if(jComfl.getSelectedIndex() == 3){
            jComzsxx.setEnabled(false);
            jListdm.setEnabled(true);
            filljdqx();
        }
        break;
      }
    }
  }

  void filldm(){
    int j;
    String strtemp = "";

    jListdm.removeAll();
    vecid.removeAllElements();
    vecdec.removeAllElements();
    vecid.addElement("全部");
    vecdec.addElement("全部");
    tablename = "站所线系参数表";
    fieldname = "代码";
    where = "";
    order = "";
    int []icount = new int[1];
    String str12 = (String)jComzsxx.getSelectedItem();
    if(str12 == null) return;
    where = "描述 = '" + str12 + "'" ;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j++)
      {
        String str1 = new String(buffer[j]);
        strtemp = str1.trim();
        break;
      }
    }
    else{
      return;
    }

    String strt = (String)jComlx.getSelectedItem();
    if(strt == null){
      return;
    }
    //待修改
    returnv = 0;
    order = "代码";
    fieldname = "代码,描述";
    where = "";
    if(strt.compareTo("YC越限") == 0){
      tablename = "模拟量视图";
    }else{
      tablename = "开关量视图";
    }

    where = "站所线系代码 = '" + strtemp + "'" ;
    returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
     byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j+=2)
      {
        String str1 = new String(buffer[j]);
        str1 = str1.trim();
        vecid.addElement(str1);
        String str2 = new String(buffer[j+1]);
        str2 = str2.trim();
        vecdec.addElement(str2);
      }
    }

    if(tablename == "开关量视图"){
      tablename = "保护信号量视图";

      where = "站所线系代码 = '" + strtemp + "'" ;
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
          order.getBytes(),icount,true);

      if(returnv > 0){
        byte [][] buffer = new byte[0][0];
        buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

        for(j = 0;j < returnv;j+=2)
        {
          String str1 = new String(buffer[j]);
          str1 = str1.trim();
          vecid.addElement(str1);
          String str2 = new String(buffer[j+1]);
          str2 = str2.trim();
          vecdec.addElement(str2);
        }
      }
    }
    if(tablename == "保护信号量视图"){
      tablename = "刀闸量视图";
      where = "站所线系代码 = '" + strtemp + "'" ;
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
          order.getBytes(),icount,true);

      if(returnv > 0){
        byte [][] buffer = new byte[0][0];
        buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

        for(j = 0;j < returnv;j+=2)
        {
          String str1 = new String(buffer[j]);
          str1 = str1.trim();
          vecid.addElement(str1);
          String str2 = new String(buffer[j+1]);
          str2 = str2.trim();
          vecdec.addElement(str2);
        }
      }
    }
    jListdm.setListData(vecdec);
    jListdm.setSelectedIndex(0);
  }

  String idtodec(String id){
    if(usacreport.FrmSacReport.g_veczsxxdec.size()<=0) return id;
    for(int i=0;i<usacreport.FrmSacReport.g_veczsxxid.size();i++){
      if(id.compareTo(usacreport.FrmSacReport.g_veczsxxid.get(i).toString())==0){
        return (String)usacreport.FrmSacReport.g_veczsxxdec.get(i);
      }
    }
    return id;
  }

  void jComlx_actionPerformed(ActionEvent e) {
    if(jComfl.getSelectedIndex()==3){
//      if("YC越限".compareTo(jComlx.getSelectedItem())==0){
        filldm();
//      }
    }
  }
}
