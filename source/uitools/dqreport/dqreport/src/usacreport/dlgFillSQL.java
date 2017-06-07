package usacreport;

import javax.swing.*;
import com.borland.jbcl.layout.XYLayout;
import com.borland.jbcl.layout.*;
import java.awt.Font;
import java.awt.Frame;
import java.awt.AWTEvent;
import java.awt.event.KeyEvent;
import java.awt.event.ActionEvent;
import java.util.Vector;
import java.util.Calendar;
import java.util.TimeZone;
import java.awt.event.ItemListener;
import java.awt.event.ItemEvent;

/**
 * Description: 报表工具
 * Company: daqo
 * SQL注入方式生成报表
 * version 1.0
 */

public class dlgFillSQL
    extends JDialog {
   public dlgFillSQL(Frame frame, String title, boolean modal) {
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

   private void jbInit() throws Exception {
      this.getContentPane().setLayout(xYLayout1);
      jButCal.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButCal.setText("退出");
      jButCal.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButCal_actionPerformed(e);
         }
      });

      jButOK.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButOK.setActionCommand("jButquit");
      jButOK.setText("填充");
      jButOK.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButOK_actionPerformed(e);
         }
      });

      xYLayout1.setWidth(483);
      xYLayout1.setHeight(368);
      jLabel1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel1.setText("库选择");
      jComdatabase.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComdatabase.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComdatabase_actionPerformed(e);
         }
      });

      jLabel2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel2.setText("终端");
      jComFac.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComFac.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComFac_actionPerformed(e);
         }
      });
      jComDec.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComDec.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComDec_actionPerformed(e);
         }
      });
      jLabel3.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel3.setText("描述");
      jLabel4.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel4.setText("代码");
      jTextname.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jTextname.setEditable(false);
      jTextname.setText("jTextField1");
      jLabel6.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel6.setText("数据类型");
      jComFlag.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComDateAttr.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel7.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel7.setToolTipText("");
      jLabel7.setText("日期属性");
      jLabel8.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel8.setText("数据格式");
      jTabbedPane.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      panel1.setLayout(xYLayout2);
      jComFormater.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComFormater.setEditable(true);
      jLabel9.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel9.setText("年");
      jComYear.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setText("日期选择");
      jLabel11.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel11.setText("月");
      jComMonth.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel12.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel12.setText("日");
      jComDay.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComHour.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel13.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel13.setText("时");
      jComMinute.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComMinute.setToolTipText("");
      jComMinute.setEditable(true);
      jLabel14.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel14.setText("分");
      jComSecond.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel15.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel15.setText("秒");
      jLabel16.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel16.setText("时间选择");
      this.setResizable(false);
      jLabel5.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel5.setText("表类型");
      jComtable.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComtable.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComdatabase_actionPerformed(e);
         }
      });

      jComFac1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComFac1.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComFac1_actionPerformed(e);
         }
      });

      panel2.setLayout(xYLayout3);
      jLabel17.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel17.setText("厂站名称");
      jChkFilter1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jChkFilter1.setSelected(true);
      jChkFilter1.setText("仅列出开关和刀闸");
      jChkFilter1.addItemListener(new ItemListener() {
         public void itemStateChanged(ItemEvent e) {
            jCheckFilter1_itemStateChanged(e);
         }
      });

      jComDec1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComDec1.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComDec_actionPerformed(e);
         }
      });

      jTextname1.setEnabled(false);
      jTextname1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jTextname1.setText("jTextField1");
      jComFlag1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel18.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel18.setText("描述");
      jLabel19.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel19.setText("代码");
      jLabel20.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel20.setText("数据类型");
      jLabel21.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel21.setText("日");
      jComMonth1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComYear1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComDay1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel22.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel22.setText("月");
      jLabel23.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel23.setText("年");
      jLabel24.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel24.setText("日期选择");
      jComFormater1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComFormater1.setEditable(true);
      jLabel25.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel25.setText("数据格式");
      jRbYx1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jRbYx1.setSelected(true);
      jRbYx1.setText("遥信");
      jRbYx1.addItemListener(new ItemListener() {
         public void itemStateChanged(ItemEvent e) {
            jRbYx1_itemStateChanged(e);
         }
      });

      jRbChan1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jRbChan1.setText("通道");
      jRbChan1.setSelected(false);
      jRbChan1.addItemListener(new ItemListener() {
         public void itemStateChanged(ItemEvent e) {
            jRbChan1_itemStateChanged(e);
         }
      });
      jLabel26.setText("数据格式段如果选择了带秒的格式,则数据库的相应字段是以秒来代表时间的");

      panel1.add(jLabel2, new XYConstraints(7, 35, -1, -1));
      panel1.add(jLabel3, new XYConstraints(7, 62, -1, -1));
      panel1.add(jLabel4, new XYConstraints(7, 91, -1, -1));
      panel1.add(jTextname, new XYConstraints(75, 88, 336, 26));
      panel1.add(jComDec, new XYConstraints(75, 60, 336, 23));
      panel1.add(jComdatabase, new XYConstraints(75, 4, 336, 23));
      panel1.add(jComFac, new XYConstraints(75, 32, 336, 23));
      panel1.add(jLabel10, new XYConstraints(6, 207, -1, -1));
      panel1.add(jLabel6, new XYConstraints(6, 153, -1, -1));
      panel1.add(jLabel11, new XYConstraints(282, 206, -1, -1));
      panel1.add(jLabel7, new XYConstraints(6, 181, -1, -1));
      panel1.add(jLabel9, new XYConstraints(178, 206, -1, -1));
      panel1.add(jLabel12, new XYConstraints(395, 206, -1, -1));
      panel1.add(jLabel15, new XYConstraints(395, 233, -1, -1));
      panel1.add(jLabel14, new XYConstraints(282, 233, -1, -1));
      panel1.add(jLabel16, new XYConstraints(6, 234, -1, -1));
      panel1.add(jLabel8, new XYConstraints(6, 259, -1, -1));
      panel1.add(jLabel1, new XYConstraints(7, 5, -1, -1));
      panel1.add(jComtable, new XYConstraints(75, 120, 336, 23));
      panel1.add(jLabel5, new XYConstraints(7, 122, -1, -1));
      panel1.add(jLabel13, new XYConstraints(178, 232, -1, -1));
      panel1.add(jComYear, new XYConstraints(74, 206, 97, 23));
      panel1.add(jComHour, new XYConstraints(74, 233, 97, 23));
      panel1.add(jComMonth, new XYConstraints(205, 206, 70, 23));
      panel1.add(jComMinute, new XYConstraints(205, 233, 70, 23));
      panel1.add(jComDay, new XYConstraints(315, 206, 72, 23));
      panel1.add(jComSecond, new XYConstraints(315, 233, 72, 23));
      panel1.add(jComFlag, new XYConstraints(74, 151, 336, 23));
      panel1.add(jComDateAttr, new XYConstraints(74, 180, 336, 23));
      panel1.add(jComFormater, new XYConstraints(74, 259, 336, 23));
      jTabbedPane.add(panel1, "单点填充");
      jTabbedPane.add(panel2, "开关填充");
      //jTabbedPane.add(panel3, "常用SQL");
      jComdatabase.addItem("遥测库");
      jComdatabase.addItem("电度库");
      Vector vecContent = new Vector();

      int dbCol = DBEx.ReadRecorders(ConstantDef.HISDB, "终端参数表", "描述", "", "描述",
                                     vecContent, true);
      if (dbCol > 0) {
         for (int i = 0; i < vecContent.size(); i += dbCol) {
            jComFac.addItem(vecContent.get(i));
            jComFac1.addItem(vecContent.get(i)); //填充面板2的终端列表
         }
      }

      //填充表类型
      jComtable.addItem("历史日表");
      jComtable.addItem("历史月表");
      //jComtable.addItem("历史年表");

      //日期属性
      jComDateAttr.addItem("选择的日期");
      jComDateAttr.addItem("选择日期的1日");
      jComDateAttr.addItem("选择日期下月1日");
      //时间
      jComYear.addItem("yyyy");
      jComYear1.addItem("yyyy");
      Calendar calTmp = Calendar.getInstance(TimeZone.getDefault());
      calTmp.add(Calendar.YEAR, -10);
      for (int i = 0; i <= 10; i++) {
      jComYear.addItem(Integer.toString(calTmp.get(Calendar.YEAR)));
      jComYear1.addItem(Integer.toString(calTmp.get(Calendar.YEAR)));
      calTmp.add(Calendar.YEAR, 1);
   }


      panel2.add(jLabel18, new XYConstraints(13, 94, -1, -1));
      panel2.add(jLabel19, new XYConstraints(13, 124, -1, -1));
      panel2.add(jLabel20, new XYConstraints(13, 153, -1, -1));
      panel2.add(jLabel24, new XYConstraints(13, 181, -1, -1));
      panel2.add(jLabel23, new XYConstraints(160, 182, 23, -1));
      panel2.add(jLabel22, new XYConstraints(258, 181, -1, -1));
      panel2.add(jLabel21, new XYConstraints(337, 182, -1, -1));
      panel2.add(jLabel25, new XYConstraints(13, 212, -1, -1));

      panel2.add(jLabel17, new XYConstraints(13, 13, 66, -1));
      panel2.add(jChkFilter1, new XYConstraints(79, 64, -1, 26));
      panel2.add(jRbChan1, new XYConstraints(150, 43, -1, 24));
      panel2.add(jRbYx1, new XYConstraints(78, 43, -1, 24));

      jComMonth.addItem("MM");
      jComMonth1.addItem("MM");
      for (int i = 1; i <= 12; i++) {
         jComMonth.addItem(Integer.toString(i));
         jComMonth1.addItem(Integer.toString(i));
      }
      jComDay.addItem("dd");
      jComDay1.addItem("dd");
      for (int i = 1; i <= 31; i++) {
         jComDay.addItem(Integer.toString(i));
         jComDay1.addItem(Integer.toString(i));
      }

      for (int i = 0; i <= 23; i++) {
         jComHour.addItem(Integer.toString(i));
      }
      for (int i = 0; i < 60; i += 5) {
         jComMinute.addItem(Integer.toString(i));
      }
      jComSecond.addItem("0");

      jComFormater.addItem("0.00");
      jComFormater.addItem("0.000");
      jComFormater.addItem("0.0%");
      jComFormater.addItem("0.00%");
      jComFormater.addItem("#0");
      jComFormater.addItem("HH:mm");
      jComFormater.addItem("HH小时mm分钟");
      jComFormater.addItem("D天HH小时mm分钟");

      jComFormater1.addItem("软件默认");
      jComFormater1.addItem("#0");
      jComFormater1.addItem("D天HH小时mm分钟");
      jComFormater1.addItem("HH:mm");
      jComFormater1.addItem("HH小时mm分ss秒");
      jComFormater1.addItem("HH:mm:ss");

      bg.add(jRbYx1);
      bg.add(jRbChan1);
      panel2.add(jComFac1, new XYConstraints(79, 12, 283, 25));
      panel2.add(jComDec1, new XYConstraints(79, 92, 283, 25));
      panel2.add(jTextname1, new XYConstraints(79, 121, 283, 25));
      panel2.add(jComFlag1, new XYConstraints(79, 150, 283, 25));
      panel2.add(jComDay1, new XYConstraints(279, 180, 53, 25));
      panel2.add(jComMonth1, new XYConstraints(186, 180, 67, 25));
      panel2.add(jComYear1, new XYConstraints(78, 180, 74, 25));
      panel2.add(jComFormater1, new XYConstraints(79, 213, 283, 25));
      panel2.add(jLabel26, new XYConstraints(11, 239, 413, 23));
      this.getContentPane().add(jButOK, new XYConstraints(242, 336, 78, -1));
      this.getContentPane().add(jButCal, new XYConstraints(370, 336, 78, -1));
      this.getContentPane().add(jTabbedPane, new XYConstraints(13, 5, 435, 327));
   }

   private XYLayout xYLayout1 = new XYLayout();
   private JButton jButCal = new JButton();
   private JButton jButOK = new JButton();
   private JTabbedPane jTabbedPane = new JTabbedPane();
   private XYLayout xYLayout2 = new XYLayout();
   private JComponent panel1 = new JPanel(false);
   private JComponent panel2 = new JPanel(false);
   //private JComponent panel3 = new JPanel(false);

   private JLabel jLabel1 = new JLabel();

   private JComboBox jComdatabase = new JComboBox();
   private JLabel jLabel2 = new JLabel();
   private JComboBox jComFac = new JComboBox();
   private JComboBox jComDec = new JComboBox();
   private JLabel jLabel3 = new JLabel();
   private JLabel jLabel4 = new JLabel();
   private JTextField jTextname = new JTextField();
   private JLabel jLabel6 = new JLabel();
   private JComboBox jComFlag = new JComboBox();
   private JComboBox jComDateAttr = new JComboBox();
   private JLabel jLabel7 = new JLabel();
   private JLabel jLabel8 = new JLabel();
   private JComboBox jComFormater = new JComboBox();
   private JLabel jLabel9 = new JLabel();
   private JComboBox jComYear = new JComboBox();
   private JLabel jLabel10 = new JLabel();
   private JLabel jLabel11 = new JLabel();
   private JComboBox jComMonth = new JComboBox();
   private JLabel jLabel12 = new JLabel();
   private JComboBox jComDay = new JComboBox();
   private JComboBox jComHour = new JComboBox();
   private JLabel jLabel13 = new JLabel();
   private JComboBox jComMinute = new JComboBox();
   private JLabel jLabel14 = new JLabel();
   private JComboBox jComSecond = new JComboBox();
   private JLabel jLabel15 = new JLabel();
   private JLabel jLabel16 = new JLabel();
   javax.swing.ButtonGroup bg = new javax.swing.ButtonGroup();

   private String strTab1SQLTemplate[] = {
       "Select data from yc where name ='VARNAME' and flag = VARFLAG  and sdate = VARDATE and time= VARTIME;fieldsShowformat:field[0]format[VARFORMAT]",
//       "Select daystopcount from kwh where name ='VARNAME' and flag = VARFLAG  and sdate = VARDATE and time= VARTIME;fieldsShowformat:field[0]format[VARFORMAT]",
//     处理电度年报表，将上面一行注释，增加如下一行
       "Select data from kwh where name ='VARNAME' and flag = VARFLAG  and sdate = VARDATE and time= VARTIME;fieldsShowformat:field[0]format[VARFORMAT]",
       "Select data from dayyc where name='VARNAME' and flag = VARFLAG and sdate = VARDATE;fieldsShowformat:field[0]format[VARFORMAT]",
       "Select otime from dayyc where name='VARNAME' and flag = VARFLAG and sdate = VARDATE;fieldsShowformat:field[0]format[VARFORMAT]",
       "Select data from daykwh where name='VARNAME' and flag = VARFLAG and sdate = VARDATE;fieldsShowformat:field[0]format[VARFORMAT]",
   };

   private String strTab2SQLTemplate[] = {
       "select dayruntime From 开关变位统计表 where name='VARNAME' and sdate=VARDATE;fieldsShowformat:field[0]format[VARFORMAT HH:mm:ss]",
       "select daystopcount From 开关变位统计表 where name='VARNAME' and sdate=VARDATE;fieldsShowformat:field[0]format[VARFORMAT #0]",
       "select sgstopcount From 开关变位统计表 where name='VARNAME' and sdate=VARDATE;fieldsShowformat:field[0]format[VARFORMAT #0]",
       "select totalruntime From 开关变位统计表 where name='VARNAME' and sdate=VARDATE;fieldsShowformat:field[0]format[VARFORMAT D日HH小时mm分钟]",
       "select totalstopcount From 开关变位统计表 where name='VARNAME' and sdate=VARDATE;fieldsShowformat:field[0]format[VARFORMAT #0]",

       "select sum(dayruntime) From 开关变位统计表 where name='VARNAME' and sdate>=[now:yyyy-MM-1] and sdate<[end:yyyy-MM-1];fieldsShowformat:field[0]format[VARFORMAT D日HH小时mm分钟]",
       "select sum(daystopcount) From 开关变位统计表 where name='VARNAME' and sdate>=[now:yyyy-MM-1] and sdate<[end:yyyy-MM-1];fieldsShowformat:field[0]format[VARFORMAT #0]",
       "select max(sgstopcount) From 开关变位统计表 where name='VARNAME' and sdate>=[now:yyyy-MM-1] and sdate<[end:yyyy-MM-1];fieldsShowformat:field[0]format[VARFORMAT #0]",


       "select 运行时间 from 设备运行统计表 where 代码='VARNAME' and 存盘日期=VARDATE;fieldsShowformat:field[0]format[HH小时mm分ss秒]",
       "select sum(运行时间) from 设备运行统计表 where 代码='VARNAME' and 存盘日期>=[now:yyyy-MM-1] and 存盘日期<[end:yyyy-MM-1];fieldsShowformat:field[0]format[VARFORMAT D日HH小时mm分ss秒]",
       "select 统计值1 from 设备运行统计表 where 代码='VARNAME' and 存盘日期=VARDATE;fieldsShowformat:field[0]format[VARFORMAT #0]",
       "select sum(统计值1) from 设备运行统计表 where 代码='VARNAME' and 存盘日期>=[now:yyyy-MM-1] and 存盘日期<[end:yyyy-MM-1];fieldsShowformat:field[0]format[VARFORMAT #0]",

   };

   private Vector vecID = new Vector();
   private Vector vecID1 = new Vector(); //面板2的

   JLabel jLabel5 = new JLabel();
   JComboBox jComtable = new JComboBox();
   JComboBox jComFac1 = new JComboBox();
   XYLayout xYLayout3 = new XYLayout();
   JLabel jLabel17 = new JLabel();
   JCheckBox jChkFilter1 = new JCheckBox();
   JComboBox jComDec1 = new JComboBox();
   JTextField jTextname1 = new JTextField();
   JComboBox jComFlag1 = new JComboBox();
   JLabel jLabel18 = new JLabel();
   JLabel jLabel19 = new JLabel();
   JLabel jLabel20 = new JLabel();
   JLabel jLabel21 = new JLabel();
   JComboBox jComMonth1 = new JComboBox();
   JComboBox jComYear1 = new JComboBox();
   JComboBox jComDay1 = new JComboBox();
   JLabel jLabel22 = new JLabel();
   JLabel jLabel23 = new JLabel();
   JLabel jLabel24 = new JLabel();
   JComboBox jComFormater1 = new JComboBox();
   JLabel jLabel25 = new JLabel();
   JRadioButton jRbYx1 = new JRadioButton();
   JRadioButton jRbChan1 = new JRadioButton();
   JLabel jLabel26 = new JLabel();

   void jComFac_actionPerformed(ActionEvent e) {
      int returnColCnt;
      Vector vecContent = new Vector();
      String where;
      where = "描述 = '" + jComFac.getSelectedItem() + "'";
      returnColCnt = DBEx.ReadRecorders(ConstantDef.MODELDB, "终端参数表", "终端序号",
                                        where, "", vecContent, true);
      if (returnColCnt <= 0) {
         jComDec.removeAllItems();
         jTextname.setText("");
         return;
      }

      jComDec.removeAllItems();
      vecID.removeAllElements();
      where = "终端序号=" + vecContent.get(0);
      if (jComdatabase.getSelectedIndex() == 0) {
         returnColCnt = DBEx.ReadRecorders(ConstantDef.MODELDB, "遥测参数表", "代码,描述",
                                           where, "", vecContent, true);
         if (returnColCnt <= 0) {
            jTextname.setText("");
            return;
         }
         for (int i = 0; i < vecContent.size(); i += returnColCnt) {
            String strTmp;
            strTmp = vecContent.get(i).toString();
            strTmp = strTmp.trim();
            vecID.addElement(strTmp);

            strTmp = vecContent.get(i + 1).toString();
            strTmp = strTmp.trim();
            jComDec.addItem(strTmp);
         }
      }
      else if (jComdatabase.getSelectedIndex() == 1) {
         returnColCnt = DBEx.ReadRecorders(ConstantDef.MODELDB, "电度参数表", "代码,描述",
                                           where, "", vecContent, true);
         if (returnColCnt <= 0) {
            jTextname.setText("");
            return;
         }
         for (int i = 0; i < vecContent.size(); i += returnColCnt) {
            String strTmp;
            strTmp = vecContent.get(i).toString();
            strTmp = strTmp.trim();
            vecID.addElement(strTmp);

            strTmp = vecContent.get(i + 1).toString();
            strTmp = strTmp.trim();
            jComDec.addItem(strTmp);
         }

      }
      jComDec.setSelectedIndex(0);
   }

   void jComFac1_actionPerformed(ActionEvent e) {
      int returnColCnt;
      Vector vecContent = new Vector();
      String where;
      where = "描述 = '" + jComFac1.getSelectedItem() + "'";
      returnColCnt = DBEx.ReadRecorders(ConstantDef.MODELDB, "终端参数表",
                                        "终端序号,主通道号",
                                        where, "", vecContent, true);
      if (returnColCnt <= 0) {
         jComDec1.removeAllItems();
         jTextname1.setText("");
         jComFlag1.removeAllItems();
         return;
      }

      jComDec1.removeAllItems();
      vecID1.removeAllElements();

      String str_1 = vecContent.get(0).toString(); //终端序号
      String str_4 = vecContent.get(0).toString(); //主通道号
      String str_2, str_3;
      if (jRbYx1.isSelected()) {
         if (jChkFilter1.isSelected()) {
            where = "终端序号=" + str_1 + " and 类型 in (300,500)";
         }
         else {
            where = "终端序号=" + str_1;
         }
         returnColCnt = DBEx.ReadRecorders(ConstantDef.MODELDB, "遥信参数表", "描述,代码",
                                           where, "", vecContent, true);
         if (returnColCnt > 0) {
            for (int i = 0; i < vecContent.size(); i += returnColCnt) {
               str_2 = vecContent.get(i).toString();
               str_3 = vecContent.get(i + 1).toString();
               str_2 = str_2.trim();
               str_3 = str_3.trim();
               jComDec1.addItem(str_2);
               vecID1.addElement(str_3); //同时保存编码
            }
            jComDec1.setSelectedIndex(0);
         }
         ItemEvent ee=null;
         jRbYx1_itemStateChanged(ee);
      }
      else if (jRbChan1.isSelected()) {
         where = "通道序号=" + str_4;
         returnColCnt = DBEx.ReadRecorders(ConstantDef.MODELDB, "通道参数表", "描述,代码",
                                           where, "", vecContent, true);
         if (returnColCnt > 0) {
            for (int i = 0; i < vecContent.size(); i += returnColCnt) {
               str_2 = vecContent.get(i).toString();
               str_3 = vecContent.get(i + 1).toString();
               str_2 = str_2.trim();
               str_3 = str_3.trim();
               jComDec1.addItem(str_2);
               vecID1.addElement(str_3); //同时保存编码
            }
            jComDec1.setSelectedIndex(0);
         }
         ItemEvent ee=null;
         jRbChan1_itemStateChanged(ee);
      }
   }

   void jComDec_actionPerformed(ActionEvent e) {
      switch (jTabbedPane.getSelectedIndex()) {
         case 0:
            if (jComDec.getItemCount() > 0) {
               if (vecID.size() > 0) {
                  jTextname.setText( (String) vecID.get(jComDec.
                      getSelectedIndex()));
               }
            }
            break;
         case 1:
            if (jComDec1.getItemCount() > 0) {
               if (vecID1.size() > 0) {
                  jTextname1.setText( (String) vecID1.get(jComDec1.
                      getSelectedIndex()));
               }
            }

            break;
      }
   }

   void jComdatabase_actionPerformed(ActionEvent e) {
      jComFlag.removeAllItems();
      jComDec.removeAllItems();
      switch (jTabbedPane.getSelectedIndex()) { //单值填充
         case 0:
            if (jComdatabase.getSelectedIndex() == 0) { //遥测库
               if (jComtable.getSelectedIndex() == 0) { //日表
                  jComFlag.addItem("某点数据[1]"); //flag <=50 data
               }
               else if (jComtable.getSelectedIndex() == 1) { //历史月表
                  jComFlag.addItem("日统计最大值[150]"); //150 data
                  jComFlag.addItem("日统计最大值发生时间[150]"); //150 otime
                  jComFlag.addItem("日统计最小值[151]"); //151 data
                  jComFlag.addItem("日统计最小值发生时间[151]"); //151 otime
                  jComFlag.addItem("日统计合格率[152]"); //152 data
                  jComFlag.addItem("日统计平均值[153]"); //153
                  jComFlag.addItem("日统计负荷率[154]"); //154  <1
                  jComFlag.addItem("日统计越上限点数[158]"); //158
                  jComFlag.addItem("日统计越下限点数[159]"); //159
                  jComFlag.addItem("日统计越上限时间[162]"); //162
                  jComFlag.addItem("日统计越下限时间[163]"); //163
                  jComFlag.addItem("日电压质量越上限点数[165]"); //165
                  jComFlag.addItem("日电压质量越下限点数[166]"); //166
                  jComFlag.addItem("日电压质量越上限时间[167]"); //167
                  jComFlag.addItem("日电压质量越下限时间[168]"); //168
                  jComFlag.addItem("日电压质量合格率[169]"); //169  <1
               }
               else if (jComtable.getSelectedIndex() == 2) { //年表

               }
            }
            else if (jComdatabase.getSelectedIndex() == 1) { //选择了电度库
               if (jComtable.getSelectedIndex() == 0) { //日表
                  jComFlag.addItem("电度表码[52]"); //52
               }
               else if (jComtable.getSelectedIndex() == 1) {
                  jComFlag.addItem("1时段日电量(平)[70]"); //70
                  jComFlag.addItem("2时段日电量(峰)[71]"); //71
                  jComFlag.addItem("3时段日电量(谷))[72]"); //72
                  jComFlag.addItem("4时段日电量(尖)[73]"); //73
                  jComFlag.addItem("5时段日电量(脊骨)[74]"); //74
                  jComFlag.addItem("6时段日电量(备用)[75]"); //75
                  jComFlag.addItem("日累计电量[79]"); //79
               }
               else if (jComtable.getSelectedIndex() == 2) {

               }
            }

            break;
         case 1:
            break;
      }
      jComFac_actionPerformed(new ActionEvent(this, 0, ""));
   }

   void jButOK_actionPerformed(ActionEvent e) {
      //开始填充SQL语句
      try {
         int m_col = usacreport.FrmSacReport.jBook1.getSelStartCol();
         int m_row = usacreport.FrmSacReport.jBook1.getSelStartRow();
         usacreport.FrmSacReport.jBook1.setActiveCell(m_row, m_col);

         switch (jTabbedPane.getSelectedIndex()) { //单值填充
            case 0:
               int nFlag = GetSelFlag();
               String strDate = GetSelDate();
               String strTime = GetSelTime();
               String strTmp = "";
               String strSQL;
               strSQL = "@SQL,";
               if (jComdatabase.getSelectedIndex() == 0) { //选择了遥测库
                  if (jComtable.getSelectedIndex() == 0) { //日表
                     strTmp = strTab1SQLTemplate[0];

                  }
                  else if (jComtable.getSelectedIndex() == 1) { //月表
                     if ( (nFlag == 150 || nFlag == 151 || nFlag == 167 ||
                           nFlag == 168) &&
                         (jComFlag.getSelectedItem()).toString().indexOf("时间") >=
                         0) {
                        strTmp = strTab1SQLTemplate[3];
                     }
                     else {
                        strTmp = strTab1SQLTemplate[2];
                     }

                  }
                  else if (jComtable.getSelectedIndex() == 2) { //年表
                     if (jComtable.getSelectedIndex() == 0) { //日表
                        strTmp = strTab1SQLTemplate[1];
                     }
                     else if (jComtable.getSelectedIndex() == 1) {
                        strTmp = strTab1SQLTemplate[4];
                     }
                  }

               }
               else { //选择了电量库
                  if (jComtable.getSelectedIndex() == 0) { //日表
                     strTmp = strTab1SQLTemplate[1];
                  }
                  else if (jComtable.getSelectedIndex() == 1) {
                     strTmp = strTab1SQLTemplate[4];
                  }
               }
               strTmp = strTmp.replaceFirst("VARNAME", jTextname.getText());
               strTmp = strTmp.replaceFirst("VARFLAG", String.valueOf(nFlag));
               strTmp = strTmp.replaceFirst("VARDATE", strDate);
               strTmp = strTmp.replaceFirst("VARTIME", strTime);
               strTmp = strTmp.replaceFirst("VARFORMAT",
                                            jComFormater.getSelectedItem().
                                            toString());
               strSQL = strSQL + strTmp;
               usacreport.FrmSacReport.jBook1.setText(m_row, m_col, strSQL);

               break;
            case 1:
                nFlag = GetSelFlag();
                strDate = GetSelDate();
                strTime = GetSelTime();
                strTmp = "";
                strSQL = "@SQL,";
                if(jRbYx1.isSelected())
                   strTmp = strTab2SQLTemplate[nFlag];
                else
                   strTmp = strTab2SQLTemplate[nFlag+8];  //遥信有8条模板.

                strTmp = strTmp.replaceFirst("VARNAME", jTextname1.getText());
                strTmp = strTmp.replaceFirst("VARDATE", strDate);
                //判断是否需要格式化
                if(jComFormater1.getSelectedIndex()!=0) //需要
                {
                   int npos1,npos2;
                   String strItem=jComFormater1.getSelectedItem().toString();
                   strItem="[" + strItem + "]";
                   npos1=strTmp.indexOf("VARFORMAT");
                   npos2=strTmp.lastIndexOf("VARFORMAT");
                   strTmp = strTmp.replaceFirst("\\[VARFORMAT.*\\]",strItem);
                }
                else
                {
                   strTmp = strTmp.replaceFirst("VARFORMAT ","");
                }
               strSQL = strSQL + strTmp;
               usacreport.FrmSacReport.jBook1.setText(m_row, m_col, strSQL);

               break;
         }

      }
      catch (com.f1j.util.F1Exception e1) {
      }
      return ;
   }

   void jButCal_actionPerformed(ActionEvent e) {
      this.hide();
      ;
   }

   public void jRbYx1_itemStateChanged(ItemEvent e) {
      if (jRbYx1.isSelected()) { //选择了遥信
         //初始化
         jChkFilter1.setEnabled(true);
         jComFlag1.removeAllItems();
         //初始化标志下拉框
         jComFlag1.addItem("日运行时间"); //12
         jComFlag1.addItem("日开关正常变位次数"); //12
         jComFlag1.addItem("日开关事故跳闸次数"); //12
         jComFlag1.addItem("开关总运行时间"); //12
         jComFlag1.addItem("开关总正常变位次数"); //12

         jComFlag1.addItem("月开关总运行时间");
         jComFlag1.addItem("月开关正常变位次数");
         jComFlag1.addItem("月开关事故跳闸次数");
      }
   }

   public void jRbChan1_itemStateChanged(ItemEvent e) {
      if (jRbChan1.isSelected()) {
         jChkFilter1.setEnabled(false);
         jComFlag1.removeAllItems();
         jComFlag1.addItem("日通道运行时间"); //1
         jComFlag1.addItem("月通道运行时间"); //1
         jComFlag1.addItem("日通道停止次数"); //3
         jComFlag1.addItem("月通道停止次数"); //3
      }
   }

   public void jCheckFilter1_itemStateChanged(ItemEvent e) {
      ActionEvent aEvent = null;
      jComFac1_actionPerformed(aEvent);
   }

   private int GetSelFlag() {
      int nFlag = 0;
      String strTmp;
      switch (jTabbedPane.getSelectedIndex()) {
         case 0:
            strTmp = jComFlag.getSelectedItem().toString();
            int pos1 = strTmp.indexOf('[');
            int pos2 = strTmp.indexOf(']');
            if (pos1 >= 0 && pos2 >= 0 && pos2 > pos1) {
               String strFlag = strTmp.substring(pos1 + 1, pos2);
               nFlag = Integer.parseInt(strFlag);
            }

            break;
         case 1:
            nFlag=jComFlag1.getSelectedIndex();  //这个选项卡的标志就是SQL模板的索引
            break;
      }

      return nFlag;
   }

   private String GetSelDate() {
      String strRet = "";
      String strYear;
      String strMonth;
      String strDay;

      switch (jTabbedPane.getSelectedIndex()) {
         case 0:
            strYear = jComYear.getSelectedItem().toString();
            strMonth = jComMonth.getSelectedItem().toString();
            strDay = jComDay.getSelectedItem().toString();
            switch (jComDateAttr.getSelectedIndex()) {
               case 0:
                  strRet = "[now:" + strYear + "-" + strMonth + "-" + strDay +
                      "]";
                  break;
               case 1:
                  strRet = "[begin:" + strYear + "-" + strMonth + "-" + strDay +
                      "]";
                  break;
               case 2:
                  strRet = "[end:" + strYear + "-" + strMonth + "-" + strDay +
                      "]";
                  break;
            }
            break;
         case 1:
            strYear=jComYear1.getSelectedItem().toString();
            strMonth = jComMonth1.getSelectedItem().toString();
            strDay = jComDay1.getSelectedItem().toString();
            strRet = "[now:" + strYear + "-" + strMonth + "-" + strDay +
                      "]";
            break;
      }
      return strRet;
   }

   private String GetSelTime() {
      String strRet="";
      switch (jTabbedPane.getSelectedIndex()) {
         case 0:
            int nHour = jComHour.getSelectedIndex();
            int nMinute = Integer.parseInt(jComMinute.getSelectedItem().
                                           toString());
            int nTime = nHour * 60 + nMinute;
            if (nTime == 0) {
               nTime = 1440;
            }
            strRet = String.valueOf(nTime);
            break;
      }
      return strRet;
   }
}
