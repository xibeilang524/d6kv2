package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.util.Vector;
import java.awt.event.*;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgModData extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JLabel jLabel2 = new JLabel();
  private JComboBox jComtem = new JComboBox();
  private JComboBox jComdec = new JComboBox();
  private JTextField jTextsxz = new JTextField();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel5 = new JLabel();
  private JTextField jTextxxz = new JTextField();
  private JButton jButOk = new JButton();
  private JButton jButCan = new JButton();
  private Vector vectortem = new Vector();
  private Vector vectorid = new Vector();
  public static Vector vectoridarr = new Vector();
  private boolean binitfin = false;
  private JLabel jLabel7 = new JLabel();
  private JTextField jTexttable = new JTextField();
  public static Vector modpara = new Vector();
  public static double max_data = 0.0;
  public static double min_data = 0.0;
  public static String str_dbname = "";

  public dlgModData(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgModData() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("终端");
    jLabel2.setText("遥测描述");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("遥测上限值");
    jLabel5.setText("遥测下限值");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jComtem.setFont(new java.awt.Font("SansSerif", 0, 12));
    jComtem.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComtem_actionPerformed(e);
      }
    });
    jComdec.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextsxz.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextxxz.setFont(new java.awt.Font("SansSerif", 0, 12));
    jButOk.setFont(new java.awt.Font("Dialog", 0, 12));
    jButOk.setText("查询");
    jButOk.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButOk_actionPerformed(e);
      }
    });
    jButCan.setFont(new java.awt.Font("Dialog", 0, 12));
    jButCan.setText("取消");
    jButCan.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButCan_actionPerformed(e);
      }
    });
    panel1.setFont(new java.awt.Font("Dialog", 0, 12));
    this.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel7.setText("历史表名");
    jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
    jTexttable.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTexttable.setText("yc200410");
    getContentPane().add(panel1);
    panel1.add(jLabel1,   new XYConstraints(32, 37, 63, -1));
    panel1.add(jLabel2,              new XYConstraints(32, 80, 76, 23));
    panel1.add(jComtem,     new XYConstraints(128, 39, 210, 23));
    panel1.add(jComdec,             new XYConstraints(128, 81, 210, 23));
    panel1.add(jTextsxz,           new XYConstraints(128, 123, 210, 23));
    panel1.add(jLabel3,           new XYConstraints(32, 123, 76, 23));
    panel1.add(jTextxxz,   new XYConstraints(128, 164, 210, 23));
    panel1.add(jLabel5,   new XYConstraints(32, 166, 76, 23));
    panel1.add(jTexttable,  new XYConstraints(128, 206, 210, 23));
    panel1.add(jLabel7,  new XYConstraints(32, 209, 76, 23));
      panel1.add(jButCan, new XYConstraints(243, 257, 95, 32));
      panel1.add(jButOk, new XYConstraints(128, 257, 95, 32));
    filltem();
  }

  void filltem()
  {
    jComtem.removeAllItems();
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
      binitfin = true;
    }
    else{
//      System.out.println("Failed to read field!");
    }
  }

  void jComtem_actionPerformed(ActionEvent e) {
    if(binitfin == false) return;

    jComdec.removeAllItems();
    int i=0;
    String dbname = "modeldb";
    String tablename = "遥测参数表";
    String fieldname = "代码,描述";
    String where = "",order = "";

    where = "终端序号 = " + (String)vectortem.get(jComtem.getSelectedIndex()*2);
    int []icount = new int[1];
    byte []bcount = new byte[1];

    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),
        where.getBytes(),order.getBytes(),icount,true);

    if(returnv > 0){
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      vectorid.removeAllElements();
      jComdec.removeAllItems();
      for(i = 0;i < m_ret;i+=2)
      {
        String str1 = new String(buffer[i]);
        str1 = str1.trim();
        String str2 = new String(buffer[i+1]);
        str2 = str2.trim();
//        System.out.println(str1+":"+str2);
        vectorid.addElement(str1);
        vectorid.addElement(str2);
        jComdec.addItem(str2);
      }
    }
    else
    {
      vectorid.removeAllElements();
      jComdec.removeAllItems();
    }
  }

  void jButCan_actionPerformed(ActionEvent e) {
    this.hide();
  }

  void jButOk_actionPerformed(ActionEvent e) {
    if(jComtem.getItemCount()==0) return;
    if(jComdec.getItemCount()==0) return;
    if(jTextsxz.getText().equals("")) return;
    if(jTextxxz.getText().equals("")) return;
    if(jTexttable.getText().equals("")) return;
    if(jTexttable.getText().substring(0,1).compareTo("y") != 0) return;//只能改非统计值,统计由存储过程自己完成

    str_dbname = jTexttable.getText().trim();
    max_data = Double.parseDouble(jTextsxz.getText().trim());
    min_data = Double.parseDouble(jTextxxz.getText().trim());
    /**
     //现在考虑每次更新yc量个数不超过20个,如果超过20个则要多次修改
     */
    int i=0;
    String dbname = "modeldb";
    String tablename = jTexttable.getText().trim();
    String fieldname = "name,sdate,time,flag,data";
    String where = "",order = "";

    where = "name = '" + (String)vectorid.get(jComdec.getSelectedIndex()*2) + "'";
    where = where + " and (data > " + jTextsxz.getText().trim()
        + " or data < " + jTextxxz.getText().trim() + ")";
    int []icount = new int[1];
    byte []bcount = new byte[1];

    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),
        where.getBytes(),order.getBytes(),icount,true);

    vectoridarr.removeAllElements();
    if(returnv > 0){
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i = 0;i < m_ret;i+=5)
      {
        String str1 = new String(buffer[i]);
        str1 = str1.trim();
        String str2 = new String(buffer[i+1]);
        str2 = str2.trim();
        String str3 = new String(buffer[i+2]);
        str3 = str3.trim();
        String str4 = new String(buffer[i+3]);
        str4 = str4.trim();
        String str5 = new String(buffer[i+4]);
        str5 = str5.trim();
        vectoridarr.addElement(str1);
        vectoridarr.addElement(str2);
        vectoridarr.addElement(str3);
        vectoridarr.addElement(str4);
        vectoridarr.addElement(str5);
      }
    }

    Dimension dlgSize = usacreport.FrmSacReport.dlgmodp1.getPreferredSize();
    Dimension frmSize = getSize();
    Point loc = getLocation();
    usacreport.FrmSacReport.dlgmodp1.setLocation((frmSize.width - dlgSize.width) / 2 + loc.x, (frmSize.height - dlgSize.height) / 2 + loc.y);
    usacreport.FrmSacReport.dlgmodp1.setModal(true);
    usacreport.FrmSacReport.dlgmodp1.pack();
    usacreport.FrmSacReport.dlgmodp1.opentableinfo();
    if(usacreport.FrmSacReport.dlgmodp1.inittext())
    {
      usacreport.FrmSacReport.dlgmodp1.show();
    }
    else
    {
      JOptionPane.showMessageDialog(this,"没有符合条件的记录!!");
//      System.out.println("没有符合条件的记录");
    }
  }
}
