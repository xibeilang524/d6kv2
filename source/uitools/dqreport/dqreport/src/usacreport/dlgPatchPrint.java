package usacreport;

import javax.swing.*;
import java.awt.BorderLayout;
import com.borland.jbcl.layout.XYLayout;
import com.borland.jbcl.layout.*;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;
import java.util.TimeZone;
import java.util.Calendar;
import java.awt.Frame;
import java.awt.AWTEvent;

/**
 * Description: 报表工具
 * Company: daqo
 * 批量打印
 * version 1.0
 */

public class dlgPatchPrint
    extends JDialog {
   public dlgPatchPrint(Frame frame, String title, boolean modal) {
      super(frame, title, modal);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);
      try {
         jbInit();
      }
      catch (Exception ex) {
    //     ex.printStackTrace();
    //     System.out.println("这里错误了");
      }
   }

   private void jbInit() throws Exception {
      this.getContentPane().setLayout(xYLayout1);
      this.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel1.setText("开始时间");
      jRadioButton9.setText("jRadioButton9");
      jRadioButton10.setText("jRadioButton10");
      jLabel2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel2.setText("年");
      jLabel3.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel3.setText("月");
      jLabel4.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel4.setText("日");
      jRadioButton1.setText("jRadioButton1");
      xYLayout1.setWidth(320);
      xYLayout1.setHeight(230);
      jLabel5.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel5.setText("日");
      jLabel6.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel6.setText("年");
      jLabel7.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel7.setText("结束时间");
      jLabel8.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel8.setText("月");
      jLabel9.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel9.setText("开始时间和结束时间间隔不能超过一个月");
      jBeginButton.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jBeginButton.setSelectedIcon(null);
      jBeginButton.setText("开始批量打印");
      jBeginButton.addActionListener(new
                                     dlgPatchPrint_jBeginButton_actionAdapter(this));
      jCancelButton.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jCancelButton.setText("取消退出");
      jCancelButton.addActionListener(new
                                      dlgPatchPrint_jCancelButton_actionAdapter(this));
      jLabel10.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setText("选择报表");
      jComSYear.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComSYear.addActionListener(new dlgPatchPrint_jComSYear_actionAdapter(this));
      jComSMonth.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComSDay.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComEYear.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComEMonth.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComEDay.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComSelRpt.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComSelRpt.addActionListener(new dlgPatchPrint_jComSelRpt_actionAdapter(this));
      this.getContentPane().add(jRadioButton9,
                                new XYConstraints(611, 63, -1, -1));
      this.getContentPane().add(jRadioButton10,
                                new XYConstraints(119, 415, -1, -1));
      this.getContentPane().add(jLabel3, new XYConstraints(219, 25, -1, -1));
      this.getContentPane().add(jLabel2, new XYConstraints(154, 24, 22, -1));
      this.getContentPane().add(jLabel1, new XYConstraints(11, 24, -1, -1));
      this.getContentPane().add(jLabel10, new XYConstraints(11, 142, 62, -1));
      this.getContentPane().add(jLabel7, new XYConstraints(11, 98, -1, -1));
      this.getContentPane().add(jLabel9, new XYConstraints(11, 63, 307, -1));
      this.getContentPane().add(jComSYear, new XYConstraints(79, 24, 70, -1));
      this.getContentPane().add(jComSMonth, new XYConstraints(175, 24, 42, -1));
      this.getContentPane().add(jComSDay, new XYConstraints(241, 24, 45, -1));
      this.getContentPane().add(jLabel4, new XYConstraints(289, 25, -1, -1));
      this.getContentPane().add(jComEYear, new XYConstraints(80, 95, 69, -1));
      this.getContentPane().add(jComEMonth, new XYConstraints(175, 95, 42, -1));
      this.getContentPane().add(jComEDay, new XYConstraints(241, 95, 45, -1));
      this.getContentPane().add(jLabel5, new XYConstraints(290, 96, -1, -1));
      this.getContentPane().add(jLabel8, new XYConstraints(219, 96, -1, -1));
      this.getContentPane().add(jLabel6, new XYConstraints(155, 96, -1, -1));
      this.getContentPane().add(jComSelRpt, new XYConstraints(79, 138, 223, 25));
      this.getContentPane().add(jCancelButton,
                                new XYConstraints(188, 176, 116, 30));
      this.getContentPane().add(jBeginButton,
                                new XYConstraints(17, 176, 116, 30));
//---hongxiang  修改日期选择
      for (int i = 2000; i < 2030; i++) {
         jComSYear.addItem(Integer.toString(i));
         jComEYear.addItem(Integer.toString(i));
      }
      Calendar cal1 = Calendar.getInstance();
      cal1.set(Calendar.ZONE_OFFSET, 28800000);

      jComSYear.setSelectedIndex(cal1.get(cal1.YEAR)-2000);
      jComEYear.setSelectedIndex(cal1.get(cal1.YEAR)-2000);
      for(int i=1;i<=12;i++)
      {
         jComSMonth.addItem(Integer.toString(i));
         jComEMonth.addItem(Integer.toString(i));
      }
      jComSMonth.setSelectedIndex(cal1.get(cal1.MONTH));
      jComEMonth.setSelectedIndex(cal1.get(cal1.MONTH));
      for(int i=1;i<=31;i++)
      {
         jComSDay.addItem(Integer.toString(i));
         jComEDay.addItem(Integer.toString(i));
      }
      jComSDay.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      jComEDay.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      //设置选择报表
      String strSQL;
      vectRtpType.clear();
      strSQL="select rptname,rpttype from webrptname where TIMEPRINTFLAG =1 ";
      Vector Content=new Vector();
      int nColCnt=DBEx.SqlReadRecorders("modeldb",strSQL,Content);
      if(nColCnt>0){
      for(int i=0;i<Content.size();i=i+nColCnt)
      {
         jComSelRpt.addItem(Content.get(i));
         if(Content.get(i+1).equals("日报表"))
         vectRtpType.add("1");
         else if (Content.get(i+1).equals("月报表"))
         vectRtpType.add("2");

      }
      if(nColCnt>0)
         jComSelRpt.setSelectedIndex(0);
      }
   }

   ButtonGroup buttonGroup1 = new ButtonGroup();
   XYLayout xYLayout1 = new XYLayout();
   JLabel jLabel1 = new JLabel();
   JRadioButton jRadioButton1 = new JRadioButton();
   JRadioButton jRadioButton9 = new JRadioButton();
   JRadioButton jRadioButton10 = new JRadioButton();
   JLabel jLabel2 = new JLabel();
   JLabel jLabel3 = new JLabel();
   JLabel jLabel4 = new JLabel();
   JLabel jLabel5 = new JLabel();
   JLabel jLabel6 = new JLabel();
   JLabel jLabel7 = new JLabel();
   JLabel jLabel8 = new JLabel();
   JLabel jLabel9 = new JLabel();
   JToggleButton jBeginButton = new JToggleButton();
   JToggleButton jCancelButton = new JToggleButton();
   JLabel jLabel10 = new JLabel();
   JComboBox jComSelRpt = new JComboBox();
   JComboBox jComSYear = new JComboBox();
   JComboBox jComSMonth = new JComboBox();
   JComboBox jComSDay = new JComboBox();
   JComboBox jComEYear = new JComboBox();
   JComboBox jComEMonth = new JComboBox();
   JComboBox jComEDay = new JComboBox();
   Vector    vectRtpType = new Vector();
   public void jBeginButton_actionPerformed(ActionEvent e) {
      if(usacreport.dlgSelect.vectorprint.isEmpty()){
         JOptionPane.showMessageDialog(null, "打印参数没有设置!", "操作提示信息",
                                            JOptionPane.INFORMATION_MESSAGE);
      }
      else
      {
         Calendar cd = Calendar.getInstance(TimeZone.getDefault());
         cd.set(Calendar.ZONE_OFFSET, 28800000);
         Calendar sDate = Calendar.getInstance(TimeZone.getDefault());
         Calendar eDate = Calendar.getInstance(TimeZone.getDefault());
         int nYear, nMonth, nDay;
         nYear = Integer.parseInt( (String) jComSYear.getSelectedItem());
         //nMonth = Integer.parseInt( (String) jComSMonth.getSelectedItem());   //hongxiang 月份会多一
         nMonth = jComSMonth.getSelectedIndex();
         nDay = Integer.parseInt( (String) jComSDay.getSelectedItem());
         sDate.set(nYear, nMonth, nDay);
         nYear = Integer.parseInt( (String) jComEYear.getSelectedItem());
         // nMonth = Integer.parseInt( (String) jComEMonth.getSelectedItem());  //hongxiang 月份会多一
         nMonth = jComEMonth.getSelectedIndex();
         nDay = Integer.parseInt( (String) jComEDay.getSelectedItem());
         eDate.set(nYear, nMonth, nDay);

         int nSelIdx = jComSelRpt.getSelectedIndex();
         FrmSacReport.dlgsel.PatchPrint(sDate, eDate,
                                        Integer.parseInt( (String) vectRtpType.
             get(nSelIdx)),
                                        (String) jComSelRpt.getSelectedItem());

      }

   }
   public void jCancelButton_actionPerformed(ActionEvent e) {
      hide();
   }

   public void jComSelRpt_actionPerformed(ActionEvent e) {

   }

   public void jComSYear_actionPerformed(ActionEvent e) {

   }

}

class dlgPatchPrint_jComSYear_actionAdapter
    implements ActionListener {
   private dlgPatchPrint adaptee;
   dlgPatchPrint_jComSYear_actionAdapter(dlgPatchPrint adaptee) {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent e) {
      adaptee.jComSYear_actionPerformed(e);
   }
}

class dlgPatchPrint_jComSelRpt_actionAdapter
    implements ActionListener {
   private dlgPatchPrint adaptee;
   dlgPatchPrint_jComSelRpt_actionAdapter(dlgPatchPrint adaptee) {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent e) {
      adaptee.jComSelRpt_actionPerformed(e);
   }
}

class dlgPatchPrint_jCancelButton_actionAdapter
    implements ActionListener {
   private dlgPatchPrint adaptee;
   dlgPatchPrint_jCancelButton_actionAdapter(dlgPatchPrint adaptee) {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent e) {
      adaptee.jCancelButton_actionPerformed(e);
   }
}

class dlgPatchPrint_jBeginButton_actionAdapter
    implements ActionListener {
   private dlgPatchPrint adaptee;
   dlgPatchPrint_jBeginButton_actionAdapter(dlgPatchPrint adaptee) {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent e) {
      adaptee.jBeginButton_actionPerformed(e);
   }
}
