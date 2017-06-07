package usacreport;

import javax.swing.*;
import com.borland.jbcl.layout.XYLayout;
import com.borland.jbcl.layout.*;
import java.awt.Font;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgFillYC
    extends JDialog {
   public dlgFillYC() {
      try {
         jbInit();
      }
      catch (Exception ex) {
         ex.printStackTrace();
      }
   }

   private void jbInit() throws Exception {
      this.getContentPane().setLayout(xYLayout1);
      jLabel1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel1.setText("类型代码");
      jComFac.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel2.setText("终端");
      jLabel3.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel3.setText("描述");
      jComboBox1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComboBox2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel4.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel4.setText("代码");
      jComDBTab.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel5.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel5.setToolTipText("");
      jLabel5.setText("日期属性");
      jTextField1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jTextField1.setEditable(false);
      jTextField1.setText("jTextField1");
      jLabel6.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel6.setText("数据类型");
      jComboBox3.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      xYLayout1.setWidth(389);
      xYLayout1.setHeight(285);
      jLabel7.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel7.setText("日期选择");
      jComboBox4.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel8.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel8.setText("年");
      jComboBox5.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel9.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel9.setText("月");
      jComboBox6.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setText("日");
      jLabel11.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel11.setText("时间选择");
      jComboBox7.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel12.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel12.setText("时");
      jComboBox8.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComboBox9.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel13.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel13.setText("分");
      jLabel14.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel14.setToolTipText("");
      jLabel14.setText("秒");
      jLabel15.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel15.setText("数据格式");
      jFormattedTextField1.setFormatterFactory(null);
      jFormattedTextField1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jFormattedTextField1.setText("jFormattedTextField1");
      jLabel16.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel16.setText("日期选择");
      jComboBox10.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      this.getContentPane().add(jLabel6, new XYConstraints(8, 148, -1, -1));
      this.getContentPane().add(jTextField1, new XYConstraints(77, 118, 283, 23));
      this.getContentPane().add(jComboBox7, new XYConstraints(77, 227, 77, -1));
      this.getContentPane().add(jComboBox4, new XYConstraints(77, 203, 77, -1));
      this.getContentPane().add(jFormattedTextField1,
                                new XYConstraints(77, 254, 283, 23));
      this.getContentPane().add(jComboBox1, new XYConstraints(77, 62, 283, 23));
      this.getContentPane().add(jComboBox10, new XYConstraints(77, 174, 283, 23));
      this.getContentPane().add(jComboBox2, new XYConstraints(77, 90, 283, 23));
      this.getContentPane().add(jComboBox3, new XYConstraints(77, 146, 283, 23));
      this.getContentPane().add(jComDBTab, new XYConstraints(77, 7, 283, 23));
      this.getContentPane().add(jComFac, new XYConstraints(77, 35, 283, 23));
      this.getContentPane().add(jLabel1, new XYConstraints(8, 120, 65, -1));

      this.getContentPane().add(jLabel15, new XYConstraints(8, 255, 60, 22));
      this.getContentPane().add(jLabel2, new XYConstraints(8, 36, 60, 22));
      this.getContentPane().add(jLabel4, new XYConstraints(8, 91, 60, 22));
      this.getContentPane().add(jLabel3, new XYConstraints(8, 63, 60, 22));
      this.getContentPane().add(jLabel5, new XYConstraints(8, 178, 60, 22));
      this.getContentPane().add(jLabel11, new XYConstraints(8, 228, 60, 22));
      this.getContentPane().add(jLabel7, new XYConstraints(8, 204, 60, 22));
      this.getContentPane().add(jLabel16, new XYConstraints(8, 8, 60, 22));
      this.getContentPane().add(jLabel12, new XYConstraints(161, 228, -1, -1));
      this.getContentPane().add(jLabel8, new XYConstraints(161, 204, -1, -1));
      this.getContentPane().add(jLabel9, new XYConstraints(256, 201, -1, -1));
      this.getContentPane().add(jLabel13, new XYConstraints(256, 228, -1, -1));
      this.getContentPane().add(jComboBox6, new XYConstraints(279, 200, 62, -1));
      this.getContentPane().add(jComboBox9, new XYConstraints(279, 227, 62, -1));
      this.getContentPane().add(jComboBox8, new XYConstraints(186, 227, 62, -1));
      this.getContentPane().add(jLabel14, new XYConstraints(342, 228, -1, -1));
      this.getContentPane().add(jLabel10, new XYConstraints(342, 201, -1, -1));
      this.getContentPane().add(jComboBox5, new XYConstraints(186, 200, 62, -1));
   }

   XYLayout xYLayout1 = new XYLayout();
   JLabel jLabel1 = new JLabel();
   JComboBox jComDBTab = new JComboBox();
   JComboBox jComFac = new JComboBox();
   JLabel jLabel2 = new JLabel();
   JLabel jLabel3 = new JLabel();
   JComboBox jComboBox1 = new JComboBox();
   JComboBox jComboBox2 = new JComboBox();
   JLabel jLabel4 = new JLabel();
   JLabel jLabel5 = new JLabel();
   JTextField jTextField1 = new JTextField();
   JLabel jLabel6 = new JLabel();
   JComboBox jComboBox3 = new JComboBox();
   JLabel jLabel7 = new JLabel();
   JComboBox jComboBox4 = new JComboBox();
   JLabel jLabel8 = new JLabel();
   JComboBox jComboBox5 = new JComboBox();
   JLabel jLabel9 = new JLabel();
   JComboBox jComboBox6 = new JComboBox();
   JLabel jLabel10 = new JLabel();
   JLabel jLabel11 = new JLabel();
   JComboBox jComboBox7 = new JComboBox();
   JLabel jLabel12 = new JLabel();
   JComboBox jComboBox8 = new JComboBox();
   JComboBox jComboBox9 = new JComboBox();
   JLabel jLabel13 = new JLabel();
   JLabel jLabel14 = new JLabel();
   JLabel jLabel15 = new JLabel();
   JFormattedTextField jFormattedTextField1 = new JFormattedTextField();
   JLabel jLabel16 = new JLabel();
   JComboBox jComboBox10 = new JComboBox();
}
