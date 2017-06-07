package usacreport;

import javax.swing.*;
import java.awt.BorderLayout;
import com.borland.jbcl.layout.XYLayout;
import com.borland.jbcl.layout.*;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Frame;
import java.awt.AWTEvent;
import java.util.Vector;

import com.f1j.util.F1Exception;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;

/**
 * Description: 报表工具
 * Company: daqo
 * 遥信填充
 * version 1.0
 */

public class dlgFillK
    extends JDialog {
   public dlgFillK(Frame frame, String title, boolean modal) {
      super(frame, title, modal);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);
      try {
         jbInit();
         pack();
      }
      catch (Exception ex) {
         ex.printStackTrace();
      }
   }
   public dlgFillK(){
      this(null, "", false);
   }

   private void jbInit() throws Exception {
      this.getContentPane().setLayout(xYLayout1);
      jLabel1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel1.setText("厂站");
      jRb2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jRb2.setText("通道");
      jRb2.addItemListener(new ItemListener() {
         public void itemStateChanged(ItemEvent e) {
            jRb2_itemStateChanged(e);
         }
      });
      jLabel2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel2.setText("描述");
      jLabel3.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel3.setText("记录");
      jTextName.setEnabled(false);
      jTextName.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel4.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel4.setText("标志");
      jBtnOK.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jBtnOK.setText("填充");
      jButCancel.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButCancel.setText("关闭");
      xYLayout1.setWidth(346);
      xYLayout1.setHeight(343);

      jComFac.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComFac_actionPerformed(e);
         }
      });

      jBtnOK.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jBtnOK_actionPerformed(e);
         }
      });

      jButCancel.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButCancel_actionPerformed(e);
         }
      });

      jComDesc.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComDesc_actionPerformed(e);
         }
      });
      jRb1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));

      jRb1.setSelected(true);
      jRb1.addItemListener(new ItemListener() {
         public void itemStateChanged(ItemEvent e) {
            jRb1_itemStateChanged(e);
         }
      });
      jComFac.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComDesc.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComFlag.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jCheckFilter.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jCheckFilter.setSelected(true);
      jCheckFilter.setText("仅列出开关和刀闸");
      jCheckFilter.addItemListener(new ItemListener() {
         public void itemStateChanged(ItemEvent e) {
            jCheckFilter_itemStateChanged(e);
         }
      });
      jRb1.setText("遥信");
      this.getContentPane().add(jComFac, new XYConstraints(80, 38, 243, -1));
      this.getContentPane().add(jRb2, new XYConstraints(174, 77, 68, -1));
      this.getContentPane().add(jRb1, new XYConstraints(81, 77, 71, -1));
      this.getContentPane().add(jLabel4, new XYConstraints(25, 262, -1, -1));
      this.getContentPane().add(jTextName, new XYConstraints(80, 204, 242, -1));
      this.getContentPane().add(jCheckFilter, new XYConstraints(77, 112, -1, -1));
      String dbname = "modeldb";
      String tablename = "终端参数表";
      String fieldname = "描述";
      String where = "", order = "描述";

      int[] icount = new int[1];
      int i = 0, j = 0;
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (returnv <= 0) {
         jComFac.removeAllItems();
   //      System.out.println("读取终端参数表失败!");
         this.hide();
      }
      else {
         byte[][] buffer = new byte[0][0];
         buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
         int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

         for (i = 0; i < returnv; i++) {
            String str_1 = new String(buffer[i]);
            str_1 = str_1.trim();
            jComFac.addItem(str_1);
         }
      }
      bg.add(jRb1);
      bg.add(jRb2);
      this.getContentPane().add(jBtnOK, new XYConstraints(80, 296, 96, 31));
      this.getContentPane().add(jButCancel, new XYConstraints(221, 296, 102, 31));
      this.getContentPane().add(jLabel2, new XYConstraints(25, 154, -1, -1));
      this.getContentPane().add(jLabel3, new XYConstraints(25, 205, -1, -1));
      this.getContentPane().add(jLabel1, new XYConstraints(25, 39, -1, -1));
      this.getContentPane().add(jComDesc, new XYConstraints(80, 146, 243, 28));
      this.getContentPane().add(jComFlag, new XYConstraints(80, 254, 242, 27));
   }

   XYLayout xYLayout1 = new XYLayout();
   JLabel jLabel1 = new JLabel();
   JComboBox jComFac = new JComboBox();
   JRadioButton jRb1 = new JRadioButton();
   JRadioButton jRb2 = new JRadioButton();
   JLabel jLabel2 = new JLabel();
   JComboBox jComDesc = new JComboBox();
   JLabel jLabel3 = new JLabel();
   JTextField jTextName = new JTextField();
   JLabel jLabel4 = new JLabel();
   JComboBox jComFlag = new JComboBox();
   JButton jBtnOK = new JButton();
   JButton jButCancel = new JButton();
   javax.swing.ButtonGroup bg = new javax.swing.ButtonGroup();

   Vector vecID = new Vector();
   JCheckBox jCheckFilter = new JCheckBox();

   public void jComFac_actionPerformed(ActionEvent e) {
      String tablename = "终端参数表";
      String fieldname = "终端序号,主通道号";
      String where = "", order = "";
      where = "描述 = '" + jComFac.getSelectedItem() + "'";
      int[] icount = new int[1];
      int i = 0, j = 0;
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(ConstantDef.
          MODELDB.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (returnv <= 0) {
         jComDesc.removeAllItems();
         jComFlag.removeAllItems();
         jTextName.setText("");
         return;
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      String str_1 = new String(buffer[0]); //终端序号
      String str_4 = new String(buffer[1]); //主通道号
      str_1=str_1.trim();
      String str_2,str_3;

      jComDesc.removeAllItems();
      vecID.removeAllElements();
      if (jRb1.isSelected()) {
         tablename = "遥信参数表";
         fieldname = "描述,代码";
         if (jCheckFilter.isSelected()) {
            where = "终端序号=" + str_1 + " and 类型 in (300,500)";
         }
         else {
            where = "终端序号=" + str_1;
         }
         returnv = usacreport.FrmSacReport.f1jdll.readRecorders(ConstantDef.MODELDB.
             getBytes(), tablename.getBytes(), fieldname.getBytes(),
             where.getBytes(),
             order.getBytes(), icount, true);
         if (returnv > 0) {
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            for (i = 0; i < returnv; i += icount[0]) {
               str_2 = new String(buffer[i]);
               str_2 = str_2.trim();
               str_3 = new String(buffer[i + 1]);
               str_3 = str_3.trim();
               jComDesc.addItem(str_2 + "  [代码为:" +str_3 + "]");
               vecID.addElement(str_3); //同时保存编码
            }
            jComDesc.setSelectedIndex(0);
         }
      }
      else if (jRb2.isSelected()) {
         tablename = "通道参数表";
         fieldname = "描述,代码";
         where = "通道序号=" + str_4;
         returnv = usacreport.FrmSacReport.f1jdll.readRecorders(ConstantDef.MODELDB.
             getBytes(), tablename.getBytes(), fieldname.getBytes(),
             where.getBytes(),
             order.getBytes(), icount, true);
         if (returnv > 0) {
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            for (i = 0; i < returnv; i += icount[0]) {
               str_2 = new String(buffer[i]);
               str_2 = str_2.trim();
               str_3 = new String(buffer[i + 1]);
               str_3 = str_3.trim();
               jComDesc.addItem(str_2 + "  [代码为:" +str_3 + "]");
               vecID.addElement(str_3); //同时保存编码
            }
            jComDesc.setSelectedIndex(0);
         }
      }

      jRb1_itemStateChanged(null);
      //vecID.removeAllElements();
      //order = "描述";
   }

   public void jBtnOK_actionPerformed(ActionEvent e) {
      try{
         int m_col = usacreport.FrmSacReport.jBook1.getSelStartCol();
         int m_row = usacreport.FrmSacReport.jBook1.getSelStartRow();
         usacreport.FrmSacReport.jBook1.setActiveCell(m_row, m_col);
         String code = "@";
         String flag = "";

         if(jComDesc.getItemCount()<=0){
            javax.swing.JOptionPane.showMessageDialog(this,"描述为空!");
            return;
         }
         if(jComFlag.getSelectedIndex()<0){
            javax.swing.JOptionPane.showMessageDialog(this,"没有选择开关状态!");
            return;
         }
         flag=(String)jComFlag.getSelectedItem();
         int beginPos=flag.indexOf("[");
         int endPos=flag.lastIndexOf("]");
         if(beginPos>0 && endPos>0 && endPos>beginPos)
            flag=flag.substring(beginPos+1,endPos);
         else{
            javax.swing.JOptionPane.showMessageDialog(this,"数据类型错误。!");
            return;
         }
         code=code + flag +"," + jTextName.getText()+",0";
         usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
      }
      catch(com.f1j.util.F1Exception  e1){
      }
      this.hide();
   }

   public void jButCancel_actionPerformed(ActionEvent e) {
      hide();
      dispose();
   }

   public void jComDesc_actionPerformed(ActionEvent e) {
      if(jComDesc.getItemCount()>0){
         if(vecID.size()>0){
            String ID=(String)vecID.get(jComDesc.getSelectedIndex());
            jTextName.setText(ID);
         }
      }
   }

   public void jCheckFilter_itemStateChanged(ItemEvent e) {
      ActionEvent aEvent=null;
      jComFac_actionPerformed(aEvent);
   }

   public void jRb1_itemStateChanged(ItemEvent e) {

      if(jRb1.isSelected()){//选择了遥信
         //初始化
         jCheckFilter.setEnabled(true);
         jComFlag.removeAllItems();
         //初始化标志下拉框
//         jComFlag.addItem("日开关总运行时间[dkz]"); //0
//         jComFlag.addItem("日开关正常变位次数[dkx]"); //1
//         jComFlag.addItem("日开关事故跳闸次数[dks]"); //2
         jComFlag.addItem("月开关总运行时间[mkz]"); //12
         jComFlag.addItem("月开关正常变位次数[mkx]"); //13
         jComFlag.addItem("月开关事故跳闸次数[mks]"); //14

/*
         jComFlag.addItem("遥调次数[ktt]"); //3
         jComFlag.addItem("遥调成功次数[ktu]"); //4
         jComFlag.addItem("遥调成功率[kv]"); //5
         jComFlag.addItem("遥控次数[kkt]"); //6
         jComFlag.addItem("遥控成功次数[kku]"); //7
         jComFlag.addItem("遥控成功率[kkv]"); //8
         jComFlag.addItem("升降次数[ksx]"); //9
         jComFlag.addItem("升降成功次数[ksu]"); //10
         jComFlag.addItem("升降成功率[ksv]"); //11
         jComFlag.addItem("拉闸限电[kxd]"); //11
*/
      }
   }

   public void jRb2_itemStateChanged(ItemEvent e) {
      if(jRb2.isSelected()){  //选择了通道
         jCheckFilter.setEnabled(false);
         jComFlag.removeAllItems();
//         jComFlag.addItem("日通道运行时间dty"); //0
         jComFlag.addItem("月通道运行时间[mty]"); //1
//         jComFlag.addItem("日通道停止次数dts"); //2
         jComFlag.addItem("月通道停止次数[mts]"); //3
      }
   }
}

