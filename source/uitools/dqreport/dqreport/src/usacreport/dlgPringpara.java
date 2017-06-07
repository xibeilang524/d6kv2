package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgPringpara extends JDialog {
  private JPanel panel1 = new JPanel();
  private JLabel jLabel1 = new JLabel();
  private XYLayout xYLayout1 = new XYLayout();
  private JComboBox jCompsize = new JComboBox();
  private JComboBox jCompora = new JComboBox();
  private JLabel jLabel2 = new JLabel();
  private JComboBox jCompnum = new JComboBox();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JButton jButok = new JButton();
  private JButton jButcancel = new JButton();
  public static short psize = 8;
  public static short pnum = 1;
  public static boolean pora = true;
  public static boolean p1page = true;
  public static boolean clickbut = true;
  private JEditorPane jEditorPane1 = new JEditorPane();
  private JComboBox jComVnum = new JComboBox();
  private JTextField jTextL = new JTextField();
  private JTextField jTextR = new JTextField();
  private JTextField jTextT = new JTextField();
  private JTextField jTextB = new JTextField();
  private JLabel jLabel6 = new JLabel();
  private JLabel jLabel7 = new JLabel();
  private JLabel jLabel8 = new JLabel();
  private JLabel jLabel9 = new JLabel();
  private JLabel jLabel5 = new JLabel();
  private JTextField jstartnum = new JTextField();

  public dlgPringpara(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgPringpara() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
     panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("纸张大小");
    jCompsize.setFont(new java.awt.Font("Dialog", 0, 12));
    jCompora.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("打印方向");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jCompnum.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("打印份数");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("打印页数");
    jButok.setFont(new java.awt.Font("Dialog", 0, 12));
    jButok.setText("打印");
    jButok.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButok_actionPerformed(e);
      }
    });
    jButcancel.setFont(new java.awt.Font("Dialog", 0, 12));
    jButcancel.setText("取消");
    jButcancel.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButcancel_actionPerformed(e);
      }
    });
    jEditorPane1.setBackground(SystemColor.activeCaptionBorder);
    jEditorPane1.setFont(new java.awt.Font("Serif", 0, 12));
    jEditorPane1.setForeground(Color.blue);
    jEditorPane1.setText("注:在这里设定横向打印为一页,纵向要打印的页数通过(打印页数)参数设置");
    jComVnum.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel6.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel6.setText("左");
    jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel7.setText("右");
    jLabel8.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel8.setText("上");
    jLabel9.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel9.setText("下");
    jTextL.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextL.setText("1.0");
    jTextT.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextT.setText("1.0");
    jTextR.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextR.setText("1.0");
    jTextB.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextB.setText("1.0");
    jLabel5.setText("页边距(CM)");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jstartnum.setText("0");
    getContentPane().add(panel1);
      panel1.add(jLabel1,   new XYConstraints(17, 16, 78, 28));
    panel1.add(jLabel2,      new XYConstraints(17, 78, 74, 28));
    panel1.add(jLabel3,       new XYConstraints(17, 139, 72, 28));
    panel1.add(jLabel4,           new XYConstraints(17, 201, 74, 28));
    panel1.add(jEditorPane1,       new XYConstraints(20, 243, 293, 53));
    panel1.add(jCompsize, new XYConstraints(108, 15, 197, 32));
    panel1.add(jCompora,   new XYConstraints(108, 71, 197, 32));
    panel1.add(jCompnum,   new XYConstraints(108, 135, 197, 32));
    panel1.add(jTextR,    new XYConstraints(241, 302, 54, 28));
    panel1.add(jLabel7,   new XYConstraints(214, 302, 19, 28));
    panel1.add(jTextL,   new XYConstraints(136, 302, 54, 28));
    panel1.add(jLabel6,   new XYConstraints(108, 302, 23, 28));
    panel1.add(jLabel5,       new XYConstraints(17, 302, 81, 28));
    panel1.add(jLabel9, new XYConstraints(214, 334, 20, 28));
    panel1.add(jLabel8, new XYConstraints(108, 334, 21, 28));
    panel1.add(jTextT, new XYConstraints(136, 334, 54, 28));
    panel1.add(jTextB, new XYConstraints(241, 334, 54, 28));
    panel1.add(jComVnum, new XYConstraints(108, 199, 197, 32));
     // panel1.add(jstartnum, new XYConstraints(155, 271, 58, 14));
      panel1.add(jButok, new XYConstraints(21, 394, 135, 36));
      panel1.add(jButcancel, new XYConstraints(178, 394, 133, 36));
      jCompsize.addItem("A4");
    jCompsize.addItem("A3");
    jCompora.addItem("横向");
    jCompora.addItem("纵向");
    int i = 0;
    for(i=1;i<6;i++){
      jCompnum.addItem(Integer.toString(i));
    }
    jComVnum.addItem("自动");
    for(i=1;i<100;i++){
      jComVnum.addItem(Integer.toString(i));
    }
    try{
      jComVnum.setSelectedIndex(1);
    }catch(java.lang.IllegalArgumentException eleagal){}
  }

  void jButok_actionPerformed(ActionEvent e) {
    switch(jCompsize.getSelectedIndex()){
      case 0:
      {
        psize = 9;
        break;
      }
      case 1:
      {
        psize = 8;
        break;
      }
    }

    switch(jCompora.getSelectedIndex()){
      case 0:
      {
        pora = true;
        break;
      }
      case 1:
      {
        pora = false;
        break;
      }
    }
    pnum = (short)(jCompnum.getSelectedIndex()+1);

    try{
      usacreport.FrmSacReport.jBook1.setPrintGridLines(false);
      if(jstartnum.getText().compareTo("0")!=0){
        usacreport.FrmSacReport.jBook1.setPrintStartPageNumber(Integer.parseInt(jstartnum.getText()));//是否可以作为不打印标题页的判断????????
      }
      usacreport.FrmSacReport.jBook1.setPrintHeader("");
      usacreport.FrmSacReport.jBook1.setPrintFooter("");
      usacreport.FrmSacReport.jBook1.setPrintLeftMargin(Double.parseDouble(jTextL.getText())/2.5);//(0.3);
      usacreport.FrmSacReport.jBook1.setPrintRightMargin(Double.parseDouble(jTextR.getText())/2.5);//单位为英寸
      usacreport.FrmSacReport.jBook1.setPrintTopMargin(Double.parseDouble(jTextT.getText())/2.5);
      usacreport.FrmSacReport.jBook1.setPrintBottomMargin(Double.parseDouble(jTextB.getText())/2.5);
      usacreport.FrmSacReport.jBook1.setPrintVCenter(true);//报表Y轴居中
      usacreport.FrmSacReport.jBook1.setPrintHCenter(true);//报表X轴居中

      usacreport.FrmSacReport.jBook1.setPrintPaperSize(psize);
      usacreport.FrmSacReport.jBook1.setPrintLandscape(pora);
      usacreport.FrmSacReport.jBook1.setPrintNumberOfCopies(pnum);
//     usacreport.FrmSacReport.jBook1.setPrintTitles(null);
//      usacreport.FrmSacReport.jBook1.setPrintScaleFitHPages(1);
//      usacreport.FrmSacReport.jBook1.setPrintScaleFitVPages(1);
//      System.out.println("打印完毕!");
//      jBook1.setPrintScaleFitToPage(true);
      if(jComVnum.getSelectedIndex()==0){
        usacreport.FrmSacReport.jBook1.setPrintScale(400);
        usacreport.FrmSacReport.jBook1.setPrintAutoPageNumbering(true);
      }else{
        usacreport.FrmSacReport.jBook1.setPrintScale(400,true,jComVnum.getSelectedIndex(),1);//400 fit to hole page
      }
      usacreport.FrmSacReport.jBook1.filePrint(false);
    }catch(com.f1j.util.F1Exception e1){
    }
    this.hide();
  }

  void jButcancel_actionPerformed(ActionEvent e) {
    clickbut = false;
    this.hide();
  }
}
