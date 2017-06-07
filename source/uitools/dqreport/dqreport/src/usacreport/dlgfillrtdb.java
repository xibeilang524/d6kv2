package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import com.f1j.util.F1Exception;
import java.util.Vector;
import java.awt.Font;
import javax.swing.border.EmptyBorder;
import java.awt.BorderLayout;

/**
 * Description: 报表工具
 * Company: daqo
 * 实时数据填充
 * version 1.0
 */

public class dlgfillrtdb extends JDialog implements ActionListener {
  private JPanel panel1 = new JPanel();
   private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel2 = new JLabel();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JLabel jLabel5 = new JLabel();
   private JComboBox jComdatabase = new JComboBox();
  private JComboBox jComFac = new JComboBox();
  private JComboBox jComDec = new JComboBox();
   private JTextField jTextname = new JTextField();
   private JButton jButOK = new JButton();
  private JButton jButCal = new JButton();
   private Vector vecID = new Vector();

  public dlgfillrtdb(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      String sfdfd = ex.getMessage();
      ex.printStackTrace();
    }
  }

  public dlgfillrtdb() {
    this(null, "", false);
  }

  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
      jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel2.setToolTipText("");
      jLabel2.setText("实时库表");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("终端");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("描述");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel5.setText("代码");
      jButOK.setFont(new java.awt.Font("Dialog", 0, 12));
    jButOK.setText("填充");
    jButOK.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButOK_actionPerformed(e);
      }
    });
    jButCal.setFont(new java.awt.Font("Dialog", 0, 12));
    jButCal.setText("取消");
    jButCal.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButCal_actionPerformed(e);
      }
    });
    jComFac.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComFac_actionPerformed(e);
      }
    });
    jComDec.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComDec_actionPerformed(e);
      }
    });
    jTextname.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextname.setEditable(false);
    jTextname.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jTextname_actionPerformed(e);
      }
    });
      jComFac.setFont(new java.awt.Font("Dialog", 0, 12));
      jComDec.setFont(new java.awt.Font("Dialog", 0, 12));
    jComdatabase.setFont(new java.awt.Font("Dialog", 0, 12));
      panel1.setBorder(new EmptyBorder(5, 5, 15, 15));
      panel1.add(jComdatabase, new XYConstraints(118, 20, 302, 24));
      panel1.add(jComFac, new XYConstraints(118, 65, 302, 24));
      panel1.add(jComDec, new XYConstraints(118, 109, 302, 24));
      panel1.add(jTextname, new XYConstraints(118, 146, 302, 24));
      panel1.add(jLabel2, new XYConstraints(33, 21, 86, 23));
      panel1.add(jButOK, new XYConstraints(34, 198, 115, 34));
      panel1.add(jLabel5, new XYConstraints(33, 146, 86, 23));
      panel1.add(jLabel4, new XYConstraints(33, 112, 86, 23));
      panel1.add(jLabel3, new XYConstraints(33, 69, 86, 23));
      panel1.add(jButCal, new XYConstraints(304, 198, 117, 34));
      this.getContentPane().add(panel1, java.awt.BorderLayout.NORTH);

      jComdatabase.addItem("模拟量表");
    jComdatabase.addItem("电度量表");

    String dbname = "modeldb";
    String tablename = "终端参数表";
    String fieldname = "描述";
    String where = "",order = "描述";

    int []icount = new int[1];
    int i = 0,j = 0;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
    if(returnv <= 0){
      jComFac.removeAllItems();
      this.hide();
    }else{
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(i=0;i<returnv;i++){
        String str_1 = new String(buffer[i]);
        str_1 = str_1.trim();
        jComFac.addItem(str_1);
      }
    }
  }

  void jButCal_actionPerformed(ActionEvent e) {
    this.hide();;
  }

  void jComFac_actionPerformed(ActionEvent e) {
    String dbname = "modeldb";
    String tablename = "终端参数表";

    String fieldname = "终端序号";
    String where = "",order = "";
    where = "描述 = '" + jComFac.getSelectedItem() + "'";

    int []icount = new int[1];
    int i = 0,j = 0;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
    if(returnv <= 0){
      jComDec.removeAllItems();
      jTextname.setText("");
      return;
    }
    byte [][] buffer = new byte[0][0];
    buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
    int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

    String str_1 = new String (buffer[0]);//终端序号
    jComDec.removeAllItems();
    vecID.removeAllElements();
    order = "描述";
    if(jComdatabase.getSelectedIndex() == 0){//遥测库
      tablename = "遥测参数表";
      fieldname = "代码,描述";
      where = "终端序号 = " + str_1;
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
      if(returnv <= 0){
        jTextname.setText("");
        return;
      }
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_3 = new String(buffer[i]);
        str_3 = str_3.trim();
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        jComDec.addItem(str_2);
        vecID.addElement(str_3);
      }
    }else{
      tablename = "电度参数表";
      fieldname = "代码,描述";
      where = "终端序号 = " + str_1;
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
      if(returnv <= 0){
        jTextname.setText("");
        return;
      }
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_3 = new String(buffer[i]);
        str_3 = str_3.trim();
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        jComDec.addItem(str_2);
        vecID.addElement(str_3);
      }
    }
    jComDec.setSelectedIndex(0);
  }

  void jComDec_actionPerformed(ActionEvent e) {
   if(jComDec.getItemCount()>0){
     if(vecID.size()>0) jTextname.setText((String)vecID.get(jComDec.getSelectedIndex()));
   }
  }

  void jButOK_actionPerformed(ActionEvent e) {

   try {
      int m_col = usacreport.FrmSacReport.jBook1.getSelStartCol();
      int m_row = usacreport.FrmSacReport.jBook1.getSelStartRow();
      usacreport.FrmSacReport.jBook1.setActiveCell(m_row, m_col);
      String code = "@rtdb,";
       if(jComdatabase.getSelectedIndex() == 0){ //是否为模拟量表
          code=code + "analog,";
       }
       else
          code=code + "kwh,";
       code =code + jTextname.getText().trim() + ",value";
       usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
   }
   catch (F1Exception ex) {
   }
  }





  protected void processWindowEvent(WindowEvent e) {
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      cancel();
    }
    super.processWindowEvent(e);
  }
  //Close the dialog
  void cancel() {
    dispose();
  }
  //Close the dialog on a button event
  public void actionPerformed(ActionEvent e) {

  }

  void jTextname_actionPerformed(ActionEvent e) {

  }

  void jComFlag_actionPerformed(ActionEvent e) {

  }
}
