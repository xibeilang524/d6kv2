package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Vector;
import java.awt.Font;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * 规律数据填充
 * version 1.0
 */

public class dlgfillgl
    extends JDialog {
   private JPanel panel1 = new JPanel();
   private XYLayout xYLayout1 = new XYLayout();
   private JRadioButton jRb1 = new JRadioButton();
   private JRadioButton jRb2 = new JRadioButton();
   private JRadioButton jRb3 = new JRadioButton();
   private JButton jButquit = new JButton();
   javax.swing.ButtonGroup bg = new javax.swing.ButtonGroup();
   private JComboBox jComdb = new JComboBox(); //库类型
   private JComboBox jComtype = new JComboBox();
   private JComboBox jComFac = new JComboBox();
    private JComboBox jComTime = new JComboBox();
   private JLabel jLabel1 = new JLabel();
   private JLabel jLabel2 = new JLabel();
   private JLabel jLabel3 = new JLabel();
    private JLabel jLabel4 = new JLabel(); //hx
   private JScrollPane jScrollPane1 = new JScrollPane();
   private JScrollPane jScrollPane2 = new JScrollPane();
   private JList jListall = new JList();
   private JList jListsel = new JList();
   private JButton jButAdd = new JButton();
   private JButton jButDel = new JButton();

    //大爷你悠着点
    private JButton jButUp = new JButton();
   private JButton jButDown = new JButton();

   private JButton jButOk = new JButton();
   public static Vector vecselgl = new Vector();
   public static Vector vecselg1l = new Vector();
   private JCheckBox jCB = new JCheckBox();
   private Vector vecid = new Vector();
   private java.util.Vector vecall = new java.util.Vector();
   JRadioButton jRb4 = new JRadioButton();
   JRadioButton jRb5 = new JRadioButton(); //月报表（表码） hx

   public dlgfillgl(Frame frame, String title, boolean modal) {
      super(frame, title, modal);
      try {
         jbInit();
         pack();
      }
      catch (Exception ex) {
         ex.printStackTrace();
      }
   }

   public dlgfillgl() {
      this(null, "", false);
   }

   private void jbInit() throws Exception {
      panel1.setLayout(xYLayout1);
      panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
      jRb1.setFont(new java.awt.Font("Dialog", 0, 12));
      jRb1.setText("日报表");
      jRb2.setFont(new java.awt.Font("Dialog", 0, 12));
      jRb2.setText("月报表");
      jRb3.setFont(new java.awt.Font("Dialog", 0, 12));
      jRb3.setText("年报表");
      jButquit.setFont(new java.awt.Font("Dialog", 0, 12));
      jButquit.setActionCommand("jButquit");
      jButquit.setText("退出");
      jButquit.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButquit_actionPerformed(e);
         }
      });
      jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel1.setText("库类型");
      jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel2.setText("数据类型");
      jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel3.setText("厂站名");
       jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
       jLabel4.setText("时间节点");

      jButAdd.setFont(new java.awt.Font("Dialog", 0, 12));
      jButAdd.setText("增加 →");
      jButAdd.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButAdd_actionPerformed(e);
         }
      });
      jButDel.setFont(new java.awt.Font("Dialog", 0, 12));
      jButDel.setText("← 删除");
      jButDel.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButDel_actionPerformed(e);
         }
      });

       jButUp.setFont(new java.awt.Font("Dialog", 0, 12));
      jButUp.setText("上移 ↑");
      jButUp.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
             jButUp_actionPerformed(e);
         }
      });
      jButDown.setFont(new java.awt.Font("Dialog", 0, 12));
      jButDown.setText("↓ 下移");
      jButDown.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
             jButDown_actionPerformed(e);
         }
      });

      this.setResizable(false);
      jComFac.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComFac_actionPerformed(e);
         }
      });
      jButOk.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButOk_actionPerformed(e);
         }
      });
       jRb5.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jRb5_actionPerformed(e);
           }
       });  //  hx
       jRb1.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jRbNormal_actionPerformed(e);
           }
       });  //  hx
       jRb2.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jRbNormal_actionPerformed(e);
           }
       });  //  hx
       jRb3.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jRbNormal_actionPerformed(e);
           }
       });  //  hx
       jRb4.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jRbNormal_actionPerformed(e);
           }
       });  //  hx
      jButOk.setText("填充");
      jButOk.setFont(new java.awt.Font("Dialog", 0, 12));
      jButOk.setActionCommand("jButquit");
      jListall.setFont(new java.awt.Font("Dialog", 0, 12));
      jListall.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      jListall.addMouseListener(new java.awt.event.MouseAdapter() {
         public void mouseClicked(MouseEvent e) {
            if (e.getClickCount() == 2) {
               OnAdd();
            }
//        jListall_mouseClicked(e);
         }
      });
      jListsel.setFont(new java.awt.Font("Dialog", 0, 12));
      jListsel.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      jListsel.addMouseListener(new java.awt.event.MouseAdapter() {
         public void mouseClicked(MouseEvent e) {
            if (e.getClickCount() == 2) {
               OnDel();
            }
//        jListsel_mouseClicked(e);
         }
      });
      jCB.setFont(new java.awt.Font("Dialog", 0, 12));
      jCB.setText("横向填充");
      jComdb.setFont(new java.awt.Font("Dialog", 0, 12));
      jComdb.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComdb_actionPerformed(e);
         }
      });
      jComFac.setFont(new java.awt.Font("Dialog", 0, 12));
      jComtype.setFont(new java.awt.Font("Dialog", 0, 12));

      jRb4.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jRb4.setActionCommand("旬报表");
      jRb4.setSelected(true);
      jRb4.setText("旬报表");

       jRb5.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
       jRb5.setActionCommand("月报表(表码)");
       jRb5.setSelected(true);
       jRb5.setText("月报表(表码)");

       jComTime.setEnabled(false);

      getContentPane().add(panel1);
      bg.add(jRb2);
      bg.add(jRb3);
      bg.add(jRb4);
      bg.add(jRb1);
       bg.add(jRb5);
      panel1.add(jScrollPane2, new XYConstraints(324, 140, 200, 230));
      jScrollPane2.getViewport().add(jListsel, null);
      panel1.add(jRb1, new XYConstraints(50, 24, 86, 26));
      panel1.add(jRb4, new XYConstraints(145, 23, -1, -1));
      panel1.add(jRb2, new XYConstraints(240, 23, 86, -1));
       panel1.add(jRb5, new XYConstraints(335, 23, -1, -1));
      panel1.add(jRb3, new XYConstraints(450, 24, 86, 26));

      panel1.add(jButAdd, new XYConstraints(232, 196, 80, 26));
       panel1.add(jButDel, new XYConstraints(232, 241, 80, 26));

       //大爷你悠着点
       panel1.add(jButUp, new XYConstraints(232, 286, 80, 26));
       panel1.add(jButDown, new XYConstraints(232, 331, 80, 26));

      panel1.add(jScrollPane1, new XYConstraints(26, 141, 198, 228));
      jScrollPane1.getViewport().add(jListall);
      panel1.add(jLabel1, new XYConstraints(26, 65, 53, 27));
      panel1.add(jLabel3, new XYConstraints(27, 97, 50, 27));
      panel1.add(jLabel2, new XYConstraints(325, 65, 66, 27));
       panel1.add(jLabel4, new XYConstraints(325, 97, 66, 27));
      panel1.add(jCB, new XYConstraints(228, 154, 88, 24));
      panel1.add(jComtype, new XYConstraints(399, 65, 123, 28));
       panel1.add(jComTime, new XYConstraints(399, 100, 123, 28));
      panel1.add(jButOk, new XYConstraints(324, 394, 80, 26));
      panel1.add(jComFac, new XYConstraints(83, 100, 140, 28));
      panel1.add(jComdb, new XYConstraints(83, 65, 140, 28));
      panel1.add(jButquit, new XYConstraints(439, 394, 80, 26));
      jRb1.setSelected(true);
      jComdb.addItem("遥测库");
      jComdb.addItem("电度库");
      try {
         jComdb.setSelectedIndex(0);
      }
      catch (java.lang.IllegalArgumentException eill) {}

      jComtype.addItem("所有类型");
      jComtype.addItem("有功功率");
      jComtype.addItem("无功功率");
      jComtype.addItem("电流");
      jComtype.addItem("电压");
      jComtype.addItem("计划数据");

       for(int i = 0; i < 24; i++){
           String timeStr = String.valueOf(i+1) + ":00";
           jComTime.addItem(timeStr);
       }

      try {
         jComtype.setSelectedIndex(0);
      }
      catch (java.lang.IllegalArgumentException eill1) {}

////////////////////////读取厂站名//////////////////////
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
   }

   void jButquit_actionPerformed(ActionEvent e) {
      this.hide();
   }

   void jComFac_actionPerformed(ActionEvent e) {
      String dbname = "modeldb";
      String tablename = "终端参数表";

      String fieldname = "终端序号";
      String where = "", order = "终端序号";

      jListall.removeAll();
      jListall.revalidate();
      vecid.removeAllElements();
      vecall.removeAllElements();
      where = "描述 = '" + jComFac.getSelectedItem() + "'";

      int[] icount = new int[1];
      int i = 0, j = 0;
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);
      if (returnv <= 0) {
         return;
      }
      byte[][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      String str_1 = new String(buffer[0]); //终端序号
      str_1 = str_1.trim();
      if (jComdb.getSelectedIndex() == 0) { //遥测库
         tablename = "遥测参数表";
         fieldname = "代码,描述";
          order = "描述";
         switch (jComtype.getSelectedIndex()) {
            case 0: {
               where = "终端序号 = " + str_1;
               break;
            }
            case 1: {
               where = "终端序号 = " + str_1 + " and 类型 >= 20 and 类型 <= 26";
               break;
            }
            case 2: {
               where = "终端序号 = " + str_1 + " and 类型 >= 40 and 类型 <= 46";
               break;
            }
            case 3: {
               where = "终端序号 = " + str_1 + " and 类型 >= 1 and 类型 <= 10";
               break;
            }
            case 4: {
               where = "终端序号 = " + str_1 + " and 类型 >= 150 and 类型 <= 159";
               break;
            }
            case 5: {
               where = "终端序号 = " + str_1 + " and 代码 = 'SsysjxP'";
               break;
            }
         }

         returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             order.getBytes(), icount, true);
//      System.out.println(where);
         if (returnv <= 0) {
            return;
         }
         buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
         m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
         for (i = 0; i < returnv; i += 2) {
            String str_3 = new String(buffer[i]); //代码
            str_3 = str_3.trim();
            vecid.addElement(str_3);
            String str_2 = new String(buffer[i + 1]); //描述
            str_2 = str_2.trim();
            vecall.addElement(str_2);
         }
      }
      else {
         tablename = "电度参数表";
         fieldname = "代码,描述";
         where = "终端序号 = " + str_1;
          order = "描述";
         returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),
             tablename.getBytes(), fieldname.getBytes(), where.getBytes(),
             order.getBytes(), icount, true);
         if (returnv <= 0) {
            return;
         }
         buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
         m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
         for (i = 0; i < returnv; i += 2) {
            String str_3 = new String(buffer[i]);
            str_3 = str_3.trim();
            vecid.addElement(str_3);
            String str_2 = new String(buffer[i + 1]);
            str_2 = str_2.trim();
            vecall.addElement(str_2);
         }
      }
      jListall.setListData(vecall);
   }

   void jButAdd_actionPerformed(ActionEvent e) {
      OnAdd();
   }

   void jButDel_actionPerformed(ActionEvent e) {
      OnDel();
   }

    void jButUp_actionPerformed(ActionEvent e) {
      OnUp();
   }

    void jButDown_actionPerformed(ActionEvent e) {
      OnDown();
   }

   void jButOk_actionPerformed(ActionEvent e) {
      FillCell();
   }

    void jRb5_actionPerformed(ActionEvent e){
        jComTime.setEnabled(true);
//        jComdb.removeItemAt(0);
        jComdb.setSelectedIndex(1);
        jComTime.setSelectedIndex(9);
    }

    void jRbNormal_actionPerformed(ActionEvent e){
        jComTime.setEnabled(false);
//        jComdb.removeItemAt(0);
        jComdb.setSelectedIndex(0);
        jComTime.setSelectedIndex(0);
    }

   void OnAdd() {
      if (jListall.getSelectedIndex() < 0) {
         return;
      }
      for (int i = 0; i < vecselgl.size(); i++) {
         if (vecselgl.get(i).toString().split( getSpace() + "//代码：")[1].equals(vecid.get(jListall.getSelectedIndex()))) {
            javax.swing.JOptionPane.showMessageDialog(this, "该测点已存在！");
            return;
         }
      }
//      vecselgl.addElement( (String) vecid.get(jListall.getSelectedIndex()));
      vecselgl.addElement(vecall.get(jListall.getSelectedIndex()).toString() + getSpace() + "//代码："
              + vecid.get(jListall.getSelectedIndex()).toString()); //
      vecselg1l.addElement(vecall.get(jListall.getSelectedIndex()).toString());
      jListsel.setListData(vecselgl);

   }

   void OnDel() {
      if (jListsel.getSelectedIndex() < 0) {
         return;
      }
      for (int i = 0; i < vecselgl.size(); i++) {
         if (vecselgl.get(i).equals(jListsel.getSelectedValue())) {
//        javax.swing.JOptionPane.showMessageDialog(this,"删除成功");
            vecselgl.remove(i);
            jListsel.setListData(vecselgl);
            return;
         }
      }
   }

    void OnUp() {
      if (jListsel.getSelectedIndex() < 0 //傻逼你没还选中要上移的元素呢，吃屎的啊！~
              || jListsel.getSelectedIndex() == 0 //傻逼你选中的是第一个元素，上移毛啊！~
              || vecselgl.size() == 1) { //傻逼列表中只有一个元素，还瞎点上移干鸟啊！~
         return;
      }
      for (int i = 0; i < vecselgl.size(); i++) {
         if (vecselgl.get(i).equals(jListsel.getSelectedValue())) {
             //第一步，更换描述
             String selectDesc = vecselg1l.get(i).toString(); //获取所选元素的描述
             String tmpDesc = vecselg1l.get(i - 1).toString(); //获取所选元素前一个元素的描述
             vecselg1l.set(i - 1, selectDesc); //所选元素上移
             vecselg1l.set(i, tmpDesc);
             //第二步，更换代码
             String selectElement = vecselgl.get(i).toString(); //获取所选元素
             String tmp = vecselgl.get(i - 1).toString(); //获取所选元素前一个元素
             vecselgl.set(i - 1, selectElement); //所选元素上移
             vecselgl.set(i, tmp);
             jListsel.setListData(vecselgl);
             break; //移好了就跳出循环，直接return太low逼了。
         }
      }
   }

    void OnDown() {
      if (jListsel.getSelectedIndex() < 0 //傻逼你还没选中要下移的元素呢，脑袋被门挤了吧！~
              || jListsel.getSelectedIndex() == vecselgl.size() - 1 //傻逼你选中的是最后一个元素，下移蛋蛋啊！~
              || vecselgl.size() == 1) { //傻逼列表中只有一个元素，下移你妹啊！~
         return;
      }
      for (int i = 0; i < vecselgl.size(); i++) {
         if (vecselgl.get(i).equals(jListsel.getSelectedValue())) {
             //第一步，更换描述
             String selectDesc = vecselg1l.get(i).toString(); //获取所选元素的描述
             String tmpDesc = vecselg1l.get(i + 1).toString(); //获取所选元素后一个元素的描述
             vecselg1l.set(i + 1,selectDesc); //所选元素上移
             vecselg1l.set(i, tmpDesc);
             //第二步，更换代码
             String selectElement = vecselgl.get(i).toString(); //获取所选元素
             String tmp = vecselgl.get(i + 1).toString(); //获取所选元素后一个元素
             vecselgl.set(i + 1, selectElement); //所选元素下移
             vecselgl.set(i, tmp);
             jListsel.setListData(vecselgl);
             break; //移好了就跳出循环，直接return太low逼了。
         }
      }
   }

    //返回一连串空格，很爽的
    String getSpace(){
        return "                              ";
    }

   void FillCell() {
      String dbname = "modeldb";
      String tablename = "遥测参数表";
      String fieldname = "代码";
      String where = "", order = "", code = "";

      if (vecselgl.size() <= 0) {
         return; //没有记录被选中
      }
      int[] icount = new int[1];
      int i = 0, j = 0;
      int returnv = 0;
      int num = 0;
      int col = usacreport.FrmSacReport.jBook1.getActiveCol() + 1;
      int row = usacreport.FrmSacReport.jBook1.getActiveRow() + 1;
      String flag = "@dh,";
      if (jComdb.getSelectedIndex() == 1) {
         tablename = "电度参数表";
         flag = "@db,";
      }
      try { ///设定需要的总行数和列数
         if (jRb1.isSelected()) {
            if (jComtype.getSelectedIndex() == 5) {
               num = 100;
            }
            else {
               num = 28;
            }
            if (jCB.isSelected()) {
               if (usacreport.FrmSacReport.jBook1.getMaxCol() < col + num) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + num + 1);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxRow() <
                   row + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
               }
            }
            else {
               if (usacreport.FrmSacReport.jBook1.getMaxRow() < row + num) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + num + 1);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxCol() <
                   col + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
               }
            }
         }
         else if (jRb4.isSelected()) {
            if (jComtype.getSelectedIndex() == 5) {
               num = 100;
            }
            else {
               num = 28;
            }
            if (jCB.isSelected()) {
               if (usacreport.FrmSacReport.jBook1.getMaxCol() < col + num) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + num + 1);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxRow() <
                   row + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
               }
            }
            else {
               if (usacreport.FrmSacReport.jBook1.getMaxRow() < row + num) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + num + 1);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxCol() <
                   col + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
               }
            }
         }
         else if (jRb2.isSelected()) {
            if (jCB.isSelected()) {
               if (usacreport.FrmSacReport.jBook1.getMaxCol() < col + 35) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + 36);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxRow() <
                   row + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
               }
            }
            else {
               if (usacreport.FrmSacReport.jBook1.getMaxRow() < row + 35) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + 36);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxCol() <
                   col + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
               }
            }
         }
         else if (jRb5.isSelected()) {
             if (jCB.isSelected()) {
                 if (usacreport.FrmSacReport.jBook1.getMaxCol() < col + 35) {
                     usacreport.FrmSacReport.jBook1.setMaxCol(col + 36);
                 }
                 if (usacreport.FrmSacReport.jBook1.getMaxRow() <
                         row + vecselgl.size()) {
                     usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
                 }
             }
             else {
                 if (usacreport.FrmSacReport.jBook1.getMaxRow() < row + 35) {
                     usacreport.FrmSacReport.jBook1.setMaxRow(row + 36);
                 }
                 if (usacreport.FrmSacReport.jBook1.getMaxCol() <
                         col + vecselgl.size()) {
                     usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
                 }
             }
         }
         else if (jRb3.isSelected()) {
            if (jCB.isSelected()) {
               if (usacreport.FrmSacReport.jBook1.getMaxCol() < col + 16) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + 17);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxRow() <
                   row + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
               }
            }
            else {
               if (usacreport.FrmSacReport.jBook1.getMaxRow() < row + 16) {
                  usacreport.FrmSacReport.jBook1.setMaxRow(row + 17);
               }
               if (usacreport.FrmSacReport.jBook1.getMaxCol() <
                   col + vecselgl.size()) {
                  usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
               }
            }
         }

         usacreport.FrmSacReport.jBook1.setRowHeight(0,
             usacreport.FrmSacReport.jBook1.getLastRow(),
             usacreport.FrmSacReport.jBook1.getRowHeight(0), false, false);
         usacreport.FrmSacReport.jBook1.setColWidth(0,
             usacreport.FrmSacReport.jBook1.getLastCol(),
             usacreport.FrmSacReport.jBook1.getColWidth(0), false);
      }
      catch (com.f1j.util.F1Exception e2) {
      }

      if (jRb1.isSelected()) {
         if (jComtype.getSelectedIndex() == 5) {
            num = 96;
         }
         else {
            num = 24;
         }
         for (i = 0; i < vecselgl.size(); i++) {
            String str_1 = (String) vecselgl.get(i);
             str_1 = str_1.split(getSpace() + "//代码：")[1];
            try {
               if (jCB.isSelected()) { //横向
                  if (i == 0) {
                     for (j = col; j < col + num; j++) {
                        usacreport.FrmSacReport.jBook1.setText(row - 1, j,
                            Integer.toString(j - col + 1));
                     }
                     if (jComtype.getSelectedIndex() != 5 && jComdb.getSelectedIndex() == 0) {
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 24, "实时最大值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 25, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 26, "实时最小值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 27, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 28, "统计最大值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 29, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 30, "统计最小值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 31, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 32, "平均值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 33, "负荷率");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 34, "合格率");
                     }
                  }

                  usacreport.FrmSacReport.jBook1.setText(row + i, col - 1,
                      (String) vecselg1l.get(i)); //描述
                  if (jComtype.getSelectedIndex() != 5) {
                     code = flag + str_1 + ",0";
                  }
                  else {
                     code = flag + str_1 + ",51";
                  }
                  usacreport.FrmSacReport.jBook1.setText(row + i, col, code);
                  usacreport.FrmSacReport.jBook1.setText(row + i, col + 1, code);
                  if (jComtype.getSelectedIndex() != 5 && jComdb.getSelectedIndex() == 0) {
                     code = "@e1," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 24,
                         code);
                     code = "@e2," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 25,
                         code);
                     code = "@e3," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 26,
                         code);
                     code = "@e4," + str_1 + ",0";

                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 27,
                         code);
                     code = "@es," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 28,
                         code);
                     code = "@et," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 29,
                         code);
                     code = "@eu," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 30,
                         code);
                     code = "@ev," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 31,
                         code);

                     code = "@ex," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 32,
                         code);
                     code = "@ey," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 33,
                         code);
                     code = "@ew," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 34,
                         code);
                  }
               }
               else { //ii%(records/6)*15  //纵向
                  if (i == 0) {
                     for (j = row; j < row + num; j++) {
                        if (jComtype.getSelectedIndex() == 5) {
                           int a = (j - row + 1) % 96 * 15 / 60;
                           String aa = Integer.toString( (j - row + 1) % 96 *
                               15 / 60);
                           if (a < 10) {
                              aa = "0" +
                                  Integer.toString( (j - row + 1) % 96 * 15 /
                                  60);
                           }
                           int b = ( (j - row + 1) % 96 * 15) % 60;
                           String bb = Integer.toString( ( (j - row + 1) % 96 *
                               15) % 60);
                           if (b < 15) {
                              bb = "0" +
                                  Integer.toString( ( (j - row + 1) % 96 * 15) %
                                  60);
                           }
                           usacreport.FrmSacReport.jBook1.setText(j, col - 1,
                               aa + ":" + bb);
                        }
                        else {
                           usacreport.FrmSacReport.jBook1.setText(j, col - 1,
                               Integer.toString(j - row + 1) + ":00");
                        }
                     }
                     if (jComtype.getSelectedIndex() != 5 && jComdb.getSelectedIndex() == 0) {
                        usacreport.FrmSacReport.jBook1.setText(row + 24,
                            col - 1, "实时最大值");
                        usacreport.FrmSacReport.jBook1.setText(row + 25,
                            col - 1, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 26,
                            col - 1, "实时最小值");
                        usacreport.FrmSacReport.jBook1.setText(row + 27,
                            col - 1, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 28,
                            col - 1, "统计最大值");
                        usacreport.FrmSacReport.jBook1.setText(row + 29,
                            col - 1, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 30,
                            col - 1, "统计最小值");
                        usacreport.FrmSacReport.jBook1.setText(row + 31,
                            col - 1, "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 32,
                            col - 1, "平均值");
                        usacreport.FrmSacReport.jBook1.setText(row + 33,
                            col - 1, "负荷率");
                        usacreport.FrmSacReport.jBook1.setText(row + 34,
                            col - 1, "合格率");
                     }
                  }

                  usacreport.FrmSacReport.jBook1.setText(row - 1, col + i,
                      (String) vecselg1l.get(i)); //描述
                  if (jComtype.getSelectedIndex() != 5) {
                     code = flag + str_1 + ",0";
                  }
                  else {
                     code = flag + str_1 + ",51";
                  }
                  usacreport.FrmSacReport.jBook1.setText(row, col + i, code);
                  usacreport.FrmSacReport.jBook1.setText(row + 1, col + i, code);
                  if (jComtype.getSelectedIndex() != 5 && jComdb.getSelectedIndex() == 0) {
                     code = "@e1," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 24, col + i,
                         code);
                     code = "@e2," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 25, col + i,
                         code);
                     code = "@e3," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 26, col + i,
                         code);
                     code = "@e4," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 27, col + i,
                         code);
                     code = "@es," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 28, col + i,
                         code);
                     code = "@et," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 29, col + i,
                         code);
                     code = "@eu," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 30, col + i,
                         code);
                     code = "@ev," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 31, col + i,
                         code);
                     code = "@ex," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 32, col + i,
                         code);
                     code = "@ey," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 33, col + i,
                         code);
                     code = "@ew," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 34, col + i,
                         code);
                  }
               }
            }
            catch (com.f1j.util.F1Exception e1) {

            }
         }
//      }
      }
      else if (jRb4.isSelected()) {
         row=0;col=0;
         try {
            usacreport.FrmSacReport.jBook1.setText(row++,col, "");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "上旬最大：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "上旬最小：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "中旬最大：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "中旬最小：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "下旬最大：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "下旬最小：");

            usacreport.FrmSacReport.jBook1.setText(row++,col, "当月最大：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "发生时间：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "当月最小：");
            usacreport.FrmSacReport.jBook1.setText(row++,col, "发生时间：");

            usacreport.FrmSacReport.jBook1.setText(row++,col, "平均值：");

            col=0;
            row=0;
            for (i = 0; i < vecselgl.size(); i++) {
               row=0;
               usacreport.FrmSacReport.jBook1.setText(row++, i+1,(String) vecselg1l.get(i)); //描述
               String str_1 = (String) vecselgl.get(i);
                str_1 = str_1.split(getSpace() + "//代码：")[1];
               code = "@xsmax," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@xsmin," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@xzmax," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@xzmin," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@xxmax," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@xxmin," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);

               code = "@ns," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@nt," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@nu," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@nv," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
               code = "@nx," + str_1+",0";
               usacreport.FrmSacReport.jBook1.setText(row++, i+1, code);
            }
         }
         catch (com.f1j.util.F1Exception e1) {

         }
      }
      else if (jRb2.isSelected()) {
         for (i = 0; i < vecselgl.size(); i++) {
            String str_1 = (String) vecselgl.get(i);
             str_1 = str_1.split(getSpace() + "//代码：")[1];
            try {
               if (jCB.isSelected()) { //横向
                  if (i == 0) {
                     for (j = col; j < col + 31; j++) {
                        usacreport.FrmSacReport.jBook1.setText(row - 1, j,
                            Integer.toString(j - col + 1) + "号");
                     }
                     if(jComdb.getSelectedIndex() == 0) {
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 31,
                            "实时最大值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 32,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 33,
                            "实时最小值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 34,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 35,
                            "统计最大值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 36,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 37,
                            "统计最小值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 38,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 39,
                            "平均值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 40,
                            "负荷率");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 41,
                            "合格率");
                     }
                  }
                  usacreport.FrmSacReport.jBook1.setText(row + i, col - 1,
                      (String) vecselg1l.get(i)); //描述
                  code = "@ms," + str_1 + ",0"; ;
                  usacreport.FrmSacReport.jBook1.setText(row + i, col, code);
                  usacreport.FrmSacReport.jBook1.setText(row + i, col + 1, code);
                  if(jComdb.getSelectedIndex() == 0) {
                     code = "@n1," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 31,
                         code);
                     code = "@n2," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 32,
                         code);
                     code = "@n3," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 33,
                         code);
                     code = "@n4," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 34,
                         code);

                     code = "@ns," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 35,
                         code);
                     code = "@nt," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 36,
                         code);
                     code = "@nu," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 37,
                         code);
                     code = "@nv," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 38,
                         code);

                     code = "@nx," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 39,
                         code);
                     code = "@ny," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 40,
                         code);
                     code = "@nw," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 41,
                         code);
                  }
               }
               else {
                  if (i == 0) {
                     for (j = row; j < row + 31; j++) {
                        usacreport.FrmSacReport.jBook1.setText(j, col - 1,
                            Integer.toString(j - row + 1));
                     }
                     if(jComdb.getSelectedIndex() == 0) {
                        usacreport.FrmSacReport.jBook1.setText(row + 31,
                            col - 1,
                            "实时最大值");
                        usacreport.FrmSacReport.jBook1.setText(row + 32,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 33,
                            col - 1,
                            "实时最小值");
                        usacreport.FrmSacReport.jBook1.setText(row + 34,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 35,
                            col - 1,
                            "统计最大值");
                        usacreport.FrmSacReport.jBook1.setText(row + 36,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 37,
                            col - 1,
                            "统计最小值");
                        usacreport.FrmSacReport.jBook1.setText(row + 38,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 39,
                            col - 1,
                            "平均值");
                        usacreport.FrmSacReport.jBook1.setText(row + 40,
                            col - 1,
                            "负荷率");
                        usacreport.FrmSacReport.jBook1.setText(row + 41,
                            col - 1,
                            "合格率");
                     }
                  }

                  usacreport.FrmSacReport.jBook1.setText(row - 1, col + i,
                      (String) vecselg1l.get(i)); //描述
                  code = "@ms," + str_1 + ",0";
                  usacreport.FrmSacReport.jBook1.setText(row, col + i, code);
                  usacreport.FrmSacReport.jBook1.setText(row + 1, col + i, code);
                  if(jComdb.getSelectedIndex() == 0) {
                     code = "@n1," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 31, col + i,
                         code);
                     code = "@n2," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 32, col + i,
                         code);
                     code = "@n3," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 33, col + i,
                         code);
                     code = "@n4," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 34, col + i,
                         code);

                     code = "@ns," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 35, col + i,
                         code);
                     code = "@nt," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 36, col + i,
                         code);
                     code = "@nu," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 37, col + i,
                         code);
                     code = "@nv," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 38, col + i,
                         code);

                     code = "@nx," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 39, col + i,
                         code);
                     code = "@ny," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 40, col + i,
                         code);
                     code = "@nw," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 41, col + i,
                         code);
                  }
               }
            }
            catch (com.f1j.util.F1Exception e1) {
            }
         }
//      }
      }
      else if (jRb5.isSelected()) {
          for (i = 0; i < vecselgl.size(); i++) {
              String str_1 = (String) vecselgl.get(i);
              str_1 = str_1.split(getSpace() + "//代码：")[1];
              try {
                  if (jCB.isSelected()) { //横向
                      if (i == 0) {
                          for (j = col; j < col + 31; j++) {
                              usacreport.FrmSacReport.jBook1.setText(row - 1, j,
                                      Integer.toString(j - col + 1) + "号");
                          }
                          if(jComdb.getSelectedIndex() == 0) {
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 31,
                                      "实时最大值");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 32,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 33,
                                      "实时最小值");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 34,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 35,
                                      "统计最大值");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 36,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 37,
                                      "统计最小值");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 38,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 39,
                                      "平均值");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 40,
                                      "负荷率");
                              usacreport.FrmSacReport.jBook1.setText(row - 1,
                                      col + 41,
                                      "合格率");
                          }
                      }
                      usacreport.FrmSacReport.jBook1.setText(row + i, col - 1,
                              (String) vecselg1l.get(i)); //描述
                      String time = "," + String.valueOf(jComTime.getSelectedIndex() + 1);
                      code = "@mbs," + str_1 + time;
                      usacreport.FrmSacReport.jBook1.setText(row + i, col, code);
                      usacreport.FrmSacReport.jBook1.setText(row + i, col + 1, code);
                      if(jComdb.getSelectedIndex() == 0) {
                          code = "@n1," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 31,
                                  code);
                          code = "@n2," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 32,
                                  code);
                          code = "@n3," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 33,
                                  code);
                          code = "@n4," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 34,
                                  code);

                          code = "@ns," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 35,
                                  code);
                          code = "@nt," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 36,
                                  code);
                          code = "@nu," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 37,
                                  code);
                          code = "@nv," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 38,
                                  code);

                          code = "@nx," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 39,
                                  code);
                          code = "@ny," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 40,
                                  code);
                          code = "@nw," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + i, col + 41,
                                  code);
                      }
                  }
                  else {
                      if (i == 0) {
                          for (j = row; j < row + 31; j++) {
                              usacreport.FrmSacReport.jBook1.setText(j, col - 1,
                                      Integer.toString(j - row + 1));
                          }
                          if(jComdb.getSelectedIndex() == 0) {
                              usacreport.FrmSacReport.jBook1.setText(row + 31,
                                      col - 1,
                                      "实时最大值");
                              usacreport.FrmSacReport.jBook1.setText(row + 32,
                                      col - 1,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row + 33,
                                      col - 1,
                                      "实时最小值");
                              usacreport.FrmSacReport.jBook1.setText(row + 34,
                                      col - 1,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row + 35,
                                      col - 1,
                                      "统计最大值");
                              usacreport.FrmSacReport.jBook1.setText(row + 36,
                                      col - 1,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row + 37,
                                      col - 1,
                                      "统计最小值");
                              usacreport.FrmSacReport.jBook1.setText(row + 38,
                                      col - 1,
                                      "发生时间");
                              usacreport.FrmSacReport.jBook1.setText(row + 39,
                                      col - 1,
                                      "平均值");
                              usacreport.FrmSacReport.jBook1.setText(row + 40,
                                      col - 1,
                                      "负荷率");
                              usacreport.FrmSacReport.jBook1.setText(row + 41,
                                      col - 1,
                                      "合格率");
                          }
                      }

                      usacreport.FrmSacReport.jBook1.setText(row - 1, col + i,
                              (String) vecselg1l.get(i)); //描述
                      String time = "," + String.valueOf(jComTime.getSelectedIndex() + 1);
                      code = "@mbs," + str_1 + time;
                      usacreport.FrmSacReport.jBook1.setText(row, col + i, code);
                      usacreport.FrmSacReport.jBook1.setText(row + 1, col + i, code);
                      if(jComdb.getSelectedIndex() == 0) {
                          code = "@n1," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 31, col + i,
                                  code);
                          code = "@n2," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 32, col + i,
                                  code);
                          code = "@n3," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 33, col + i,
                                  code);
                          code = "@n4," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 34, col + i,
                                  code);

                          code = "@ns," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 35, col + i,
                                  code);
                          code = "@nt," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 36, col + i,
                                  code);
                          code = "@nu," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 37, col + i,
                                  code);
                          code = "@nv," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 38, col + i,
                                  code);

                          code = "@nx," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 39, col + i,
                                  code);
                          code = "@ny," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 40, col + i,
                                  code);
                          code = "@nw," + str_1 + ",0";
                          usacreport.FrmSacReport.jBook1.setText(row + 41, col + i,
                                  code);
                      }
                  }
              }
              catch (com.f1j.util.F1Exception e1) {
              }
          }
//      }
      }
      else if (jRb3.isSelected()) {
         for (i = 0; i < vecselgl.size(); i++) {
            String str_1 = (String) vecselgl.get(i);
             str_1 = str_1.split(getSpace() + "//代码：")[1];
            try {
               if (jCB.isSelected()) { //横向
                  if (i == 0) {
                     for (j = col; j < col + 12; j++) {
                        usacreport.FrmSacReport.jBook1.setText(row - 1, j,
                            Integer.toString(j - col + 1));
                     }
                     if(jComdb.getSelectedIndex() == 0) {
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 12,
                            "实时最大值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 13,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 14,
                            "实时最小值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 15,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 16,
                            "统计最大值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 17,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 18,
                            "统计最小值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 19,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 20,
                            "平均值");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 21,
                            "负荷率");
                        usacreport.FrmSacReport.jBook1.setText(row - 1,
                            col + 22,
                            "合格率");
                     }
                  }
                  usacreport.FrmSacReport.jBook1.setText(row + i, col - 1,
                      (String) vecselg1l.get(i)); //描述
                  code = "@ys," + str_1 + ",0";
                  usacreport.FrmSacReport.jBook1.setText(row + i, col, code);
                  usacreport.FrmSacReport.jBook1.setText(row + i, col + 1, code);
                  if(jComdb.getSelectedIndex() == 0) {
                     code = "@z1," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 12,
                         code);
                     code = "@z2," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 13,
                         code);
                     code = "@z3," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 14,
                         code);
                     code = "@z4," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 15,
                         code);

                     code = "@zs," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 16,
                         code);
                     code = "@zt," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 17,
                         code);
                     code = "@zu," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 18,
                         code);
                     code = "@zv," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 19,
                         code);

                     code = "@zx," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 20,
                         code);
                     code = "@zy," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 21,
                         code);
                     code = "@zw," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + i, col + 22,
                         code);
                  }
               }
               else {
                  if (i == 0) {
                     for (j = row; j < row + 12; j++) {
                        usacreport.FrmSacReport.jBook1.setText(j, col - 1,
                            Integer.toString(j - row + 1));
                     }
                     if(jComdb.getSelectedIndex() == 0) {
                        usacreport.FrmSacReport.jBook1.setText(row + 12,
                            col - 1,
                            "实时最大值");
                        usacreport.FrmSacReport.jBook1.setText(row + 13,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 14,
                            col - 1,
                            "实时最小值");
                        usacreport.FrmSacReport.jBook1.setText(row + 15,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 16,
                            col - 1,
                            "统计最大值");
                        usacreport.FrmSacReport.jBook1.setText(row + 17,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 18,
                            col - 1,
                            "统计最小值");
                        usacreport.FrmSacReport.jBook1.setText(row + 19,
                            col - 1,
                            "发生时间");
                        usacreport.FrmSacReport.jBook1.setText(row + 20,
                            col - 1,
                            "平均值");
                        usacreport.FrmSacReport.jBook1.setText(row + 21,
                            col - 1,
                            "负荷率");
                        usacreport.FrmSacReport.jBook1.setText(row + 22,
                            col - 1,
                            "合格率");
                     }
                  }
                  usacreport.FrmSacReport.jBook1.setText(row - 1, col + i,
                      (String) vecselg1l.get(i)); //描述
                  code = "@ys," + str_1 + ",0";
                  usacreport.FrmSacReport.jBook1.setText(row, col + i, code);
                  usacreport.FrmSacReport.jBook1.setText(row + 1, col + i, code);
                  if(jComdb.getSelectedIndex() == 0) {
                     code = "@z1," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 12, col + i,
                         code);
                     code = "@z2," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 13, col + i,
                         code);
                     code = "@z3," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 14, col + i,
                         code);
                     code = "@z4," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 15, col + i,
                         code);

                     code = "@zs," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 16, col + i,
                         code);
                     code = "@zt," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 17, col + i,
                         code);
                     code = "@zu," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 18, col + i,
                         code);
                     code = "@zv," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 19, col + i,
                         code);

                     code = "@zx," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 20, col + i,
                         code);
                     code = "@zy," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 21, col + i,
                         code);
                     code = "@zw," + str_1 + ",0";
                     usacreport.FrmSacReport.jBook1.setText(row + 22, col + i,
                         code);
                  }
               }
            }
            catch (com.f1j.util.F1Exception e1) {
            }
         }
//     }
      }
      vecselgl.removeAllElements();
      vecselg1l.removeAllElements();
      this.hide();
   }

   void jComdb_actionPerformed(ActionEvent e) {
      jListall.removeAll();
      jListall.revalidate();
      vecid.removeAllElements();
      vecall.removeAllElements();
   }
}
