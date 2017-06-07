package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Calendar;
import java.util.Vector;
import java.awt.BorderLayout;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgExport extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JLabel jLabel2 = new JLabel();
  private JComboBox jComyearb = new JComboBox();
  private JComboBox jCommonb = new JComboBox();
  private JComboBox jComdayb = new JComboBox();
  private JComboBox jComyeare = new JComboBox();
  private JComboBox jCommone = new JComboBox();
  private JComboBox jComdaye = new JComboBox();
  private JButton jButOK = new JButton();
  private JButton jButCan = new JButton();
  String dbname = "modeldb";
  String tablename = "",tbname = "";
  String fieldname = "";
  String where = "",order = "";
  private JLabel jLabel8 = new JLabel();
  private JLabel jLabel10 = new JLabel();
  private JLabel jLabel14 = new JLabel();
  private JLabel jLabel15 = new JLabel();
  private JLabel jLabel16 = new JLabel();
  private JLabel jLabel17 = new JLabel();
  private JLabel jLabel9 = new JLabel();
  private JComboBox jComfz1 = new JComboBox();
  private Vector vecid = new Vector();
  private Vector vecdec = new Vector();
  private Vector veceventzsxx = new Vector();
  private Calendar calbegin = Calendar.getInstance();
  private Calendar calend = Calendar.getInstance();

  private Vector datavec = new Vector();
  public static boolean threadisrun = false;
  private static boolean searchend = true;

  public dlgExport(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgExport() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    calbegin.set(Calendar.ZONE_OFFSET, 28800000);
    calend.set(Calendar.ZONE_OFFSET, 28800000);

    panel1.setLayout(xYLayout1);
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("起始时间");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("终止时间");
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
    jLabel8.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel8.setText("年");
    jLabel10.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel10.setToolTipText("");
    jLabel10.setText("月");

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
    jComfz1.setFont(new java.awt.Font("Dialog", 0, 12));
    panel1.add(jLabel1,  new XYConstraints(11, 17, 60, 24));
    panel1.add(jLabel2,   new XYConstraints(11, 64, 60, 24));
    panel1.add(jComyearb,   new XYConstraints(86, 20, 63, -1));
    panel1.add(jComyeare,   new XYConstraints(86, 66, 63, -1));
    panel1.add(jComdaye,          new XYConstraints(234, 66, 45, 27));
    panel1.add(jComdayb,        new XYConstraints(234, 20, 45, 27));
    panel1.add(jCommonb,   new XYConstraints(171, 20, 45, 27));
    panel1.add(jCommone,      new XYConstraints(171, 66, 45, 27));
      panel1.add(jButOK, new XYConstraints(11, 148, 129, 32));
      panel1.add(jButCan, new XYConstraints(162, 146, 130, 34));
      panel1.add(jLabel15,    new XYConstraints(220, 66, -1, 27));
    panel1.add(jLabel10,    new XYConstraints(220, 20, -1, 27));
    panel1.add(jLabel16,    new XYConstraints(282, 20, -1, 27));
    panel1.add(jLabel17,     new XYConstraints(282, 66, -1, 27));
    panel1.add(jComfz1,        new XYConstraints(86, 106, 202, -1));
    panel1.add(jLabel9,   new XYConstraints(11, 106, 60, 24));
    panel1.add(jLabel14,   new XYConstraints(151, 66, 17, 27));
    panel1.add(jLabel8,   new XYConstraints(151, 20, 17, 27));
      this.getContentPane().add(panel1, java.awt.BorderLayout.NORTH);

    int i;
    for(i=2000;i<2010;i++)
    {
      jComyearb.addItem(Integer.toString(i));
      jComyeare.addItem(Integer.toString(i));
    }
    for(i=1;i<13;i++)
    {
      jCommonb.addItem(Integer.toString(i));
      jCommone.addItem(Integer.toString(i));
    }


    dayfill(1,0);
    Calendar cal1 = Calendar.getInstance();
    cal1.set(Calendar.ZONE_OFFSET, 28800000);

    try{
      jComyearb.setSelectedIndex(cal1.get(cal1.YEAR)-2000);
      jComyeare.setSelectedIndex(cal1.get(cal1.YEAR)-2000);
      jCommonb.setSelectedIndex(cal1.get(cal1.MONTH));
      jCommone.setSelectedIndex(cal1.get(cal1.MONTH));
      jComdayb.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      jComdaye.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
    }catch(java.lang.IllegalArgumentException elegal){}
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
          if((jComyearb.getSelectedIndex()+2000)%4!=0)
          {
            for(i=1;i<29;i++)
              {
                  jComdayb.addItem(Integer.toString(i));
              }
          }
          else  if((jComyearb.getSelectedIndex()+2000)%100!=0)
          {
              for(i=1;i<30;i++)
              {
                  jComdayb.addItem(Integer.toString(i));
              }
          }
          else  if((jComyearb.getSelectedIndex()+2000)%400!=0)
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
          if((jComyeare.getSelectedIndex()+2000)%4!=0)
          {
            for(i=1;i<29;i++)
              {
                  jComdaye.addItem(Integer.toString(i));
              }
          }
          else  if((jComyeare.getSelectedIndex()+2000)%100!=0)
          {
              for(i=1;i<30;i++)
              {
                  jComdaye.addItem(Integer.toString(i));
              }
          }
          else  if((jComyeare.getSelectedIndex()+2000)%400!=0)
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





  void jComzsxx_actionPerformed(ActionEvent e) {

  }



  void jButOK_actionPerformed(ActionEvent e) {

  }

  void jComfz1_actionPerformed(ActionEvent e) {

  }




}
