package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import com.f1j.util.F1Exception;
import java.util.Vector;
import java.awt.Font;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * 遥测填充
 * version 1.0
 */

public class dlgFill extends JDialog implements ActionListener {
  private JPanel panel1 = new JPanel();
  private JLabel jLabel1 = new JLabel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel2 = new JLabel();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JLabel jLabel5 = new JLabel();
  private JLabel jLabel6 = new JLabel();
  private JComboBox jComtype = new JComboBox();
  private JComboBox jComdatabase = new JComboBox();
  private JComboBox jComFac = new JComboBox();
  private JComboBox jComDec = new JComboBox();
  private JComboBox jComFlag = new JComboBox();
  private JTextField jTextname = new JTextField();
  private JCheckBox jCheckgl = new JCheckBox();
  private JCheckBox jCheckDir = new JCheckBox();
  private JButton jButOK = new JButton();
  private JButton jButCal = new JButton();
  private JLabel jLabel7 = new JLabel();
  private JComboBox jComhour = new JComboBox();
  private Vector vecID = new Vector();

  public dlgFill(Frame frame, String title, boolean modal) {
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

  public dlgFill() {
    this(null, "", false);
  }

  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    jLabel1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
    jLabel1.setText("报表类型");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("遥测类型");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("终端");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("描述");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel5.setText("代码");
    jLabel6.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel6.setText("数据类型");
    jCheckgl.setFont(new java.awt.Font("Dialog", 0, 12));
    jCheckgl.setText("规律数据");
    jCheckDir.setFont(new java.awt.Font("Dialog", 0, 12));
    jCheckDir.setText("纵向数据");
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
    jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel7.setText("小时");
    jComdatabase.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComdatabase_actionPerformed(e);
      }
    });
    jTextname.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextname.setEditable(false);
    jTextname.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jTextname_actionPerformed(e);
      }
    });
    jComtype.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComtype_actionPerformed(e);
      }
    });
    jComFac.setFont(new java.awt.Font("Dialog", 0, 12));
    jComFlag.setFont(new java.awt.Font("Dialog", 0, 12));
    jComFlag.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComFlag_actionPerformed(e);
      }
    });
    jComDec.setFont(new java.awt.Font("Dialog", 0, 12));
    jComdatabase.setFont(new java.awt.Font("Dialog", 0, 12));
    jComtype.setFont(new java.awt.Font("Dialog", 0, 12));
    jComhour.setFont(new java.awt.Font("Dialog", 0, 12));
    getContentPane().add(panel1);
    panel1.setBorder(new EmptyBorder(5, 5, 15, 15));
      panel1.add(jLabel4, new XYConstraints(22, 142, 86, 23));
    panel1.add(jLabel5, new XYConstraints(22, 183, 86, 23));
    panel1.add(jComFac, new XYConstraints(119, 101, 302, 24));
    panel1.add(jLabel3, new XYConstraints(22, 101, 86, 23));
    panel1.add(jComFlag, new XYConstraints(119, 224, 302, 24));
    panel1.add(jComDec, new XYConstraints(119, 142, 302, 24));
    panel1.add(jComdatabase, new XYConstraints(119, 60, 302, 24));
    panel1.add(jLabel2, new XYConstraints(22, 60, 86, 23));
      panel1.add(jComtype, new XYConstraints(119, 19, 302, 24));
    panel1.add(jTextname, new XYConstraints(119, 183, 302, 24));
      panel1.add(jCheckDir, new XYConstraints(135, 267, 108, 25));
    panel1.add(jCheckgl, new XYConstraints(22, 267, 108, 25));
      panel1.add(jComhour, new XYConstraints(342, 269, 79, 25));
      panel1.add(jButCal, new XYConstraints(304, 315, 117, 34));
      panel1.add(jButOK, new XYConstraints(119, 316, 115, 34));
      panel1.add(jLabel6, new XYConstraints(22, 222, 86, 23));
      panel1.add(jLabel1, new XYConstraints(22, 19, 86, 23));
      panel1.add(jLabel7, new XYConstraints(317, 271, 46, 24));
      jComtype.addItem("日报表");
    jComtype.addItem("月报表");
    jComtype.addItem("年报表");
    jComdatabase.addItem("遥测库");
    jComdatabase.addItem("电度库");

    String dbname = "modeldb";
    String tablename = "终端参数表";
    String fieldname = "描述";
    String where = "";
      String order = "描述";

    int []icount = new int[1];
    int i = 0,j = 0;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
    if(returnv <= 0){
      jComFac.removeAllItems();
//      System.out.println("读取终端参数表失败!");
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
//      System.out.println("读取终端参数表成功!");
    }

/*    jComFlag.addItem("整点数据");
    jComFlag.addItem("日统计最大值");//s
    jComFlag.addItem("日统计最大值发生时间");//t
    jComFlag.addItem("日统计最小值");//u
    jComFlag.addItem("日统计最小值发生时间");//v
    jComFlag.addItem("日统计合格率");//w
    jComFlag.addItem("日统计平均值");//x
    jComFlag.addItem("日统计负荷率");//y
    jComFlag.addItem("日统计越上限点数)");//z
    jComFlag.addItem("日统计越下限点数)");//11
    jComFlag.addItem("日统计越上限时间(");//12
    jComFlag.addItem("日统计越下限时间");//13
    jComFlag.addItem("日电压质量越上限点数");//14
    jComFlag.addItem("日电压质量越下限点数");//15
    jComFlag.addItem("日电压质量越上限时间");//16
    jComFlag.addItem("日电压质量越下限时间");//17
    jComFlag.addItem("日电压质量合格率)");//18
    jComFlag.addItem("整点最大值时间");//maxtime
    jComFlag.addItem("整点最小值时间");//mintime
    jComFlag.addItem("实时最大值");//1
    jComFlag.addItem("实时最大值发生时间");//2
    jComFlag.addItem("实时最小值)");//3
    jComFlag.addItem("实时最小值发生时间");//4
    jComFlag.addItem("实时合格率");//5
    jComFlag.addItem("实时平均值");//6
    jComFlag.addItem("实时负荷率");//7
    jComFlag.addItem("总越上上限次数");//8
    jComFlag.addItem("总越上限次数");//9
    jComFlag.addItem("总越下限次数");//d
    jComFlag.addItem("总越下下限次数");//e
    jComFlag.addItem("总越上上限时间");//f
    jComFlag.addItem("总越上限时间");//g
    jComFlag.addItem("总越下限时间");//h
    jComFlag.addItem("总越下下限时间");//i
    jComFlag.addItem("日死数时间)");//q
    jComFlag.addItem("日停运时间");//r
    jComFlag.addItem("最大值相关数据");
    jComFlag.addItem("最小值相关数据");
*/
    /**
     jComFlag.addItem("整点数据(<50)");
    jComFlag.addItem("计划值(51)");//a
    jComFlag.addItem("电度表码(52)");//b
    jComFlag.addItem("电度原始数据(53)");//c
//4
    jComFlag.addItem("整点最大值时间(24点)");//maxtime
    jComFlag.addItem("整点最小值时间(24点)");//mintime
    jComFlag.addItem("最大值(150)");//1
    jComFlag.addItem("最大值发生时间(150)");//2
    jComFlag.addItem("最小值(151)");//3
    jComFlag.addItem("最小值发生时间(151)");//4
    jComFlag.addItem("合格率(152)");//5
    jComFlag.addItem("平均值(153)");//6
    jComFlag.addItem("负荷率(154)");//7

//13
    jComFlag.addItem("越上上限次数(62)");//8
    jComFlag.addItem("越上限次数(63)");//9
    jComFlag.addItem("越下限次数(64)");//d
    jComFlag.addItem("越下下限次数(65)");//e
    jComFlag.addItem("越上上限时间(66)");//f
    jComFlag.addItem("越上限时间(67)");//g
    jComFlag.addItem("越下限时间(68)");//h
    jComFlag.addItem("越下下限时间(69)");//i
    jComFlag.addItem("1时段日电量(平)(70)");//j
    jComFlag.addItem("2时段日电量(峰)(71)");//k
    jComFlag.addItem("3时段日电量(谷)(72)");//l
    jComFlag.addItem("4时段日电量(尖)(73)");//m
    jComFlag.addItem("5时段日电量(脊骨)(74)");//n
    jComFlag.addItem("6时段日电量(75)");//o
//27
    jComFlag.addItem("日累计电量(79)");//p
//28
    jComFlag.addItem("日死数时间(80)");//q
    jComFlag.addItem("日停运时间(81)");//r
//30
    jComFlag.addItem("日统计最大值(150)");//s
    jComFlag.addItem("日统计最大值发生时间(150)");//t
    jComFlag.addItem("日统计最小值(151)");//u
    jComFlag.addItem("日统计最小值发生时间(151)");//v
    jComFlag.addItem("日统计合格率(152)");//w
    jComFlag.addItem("日统计平均值(153)");//x
    jComFlag.addItem("日统计负荷率(154)");//y
//37
    jComFlag.addItem("日统计越上限点数(158)");//z
    jComFlag.addItem("日统计越下限点数(159)");//11
    jComFlag.addItem("日统计越上限时间(162)");//12
    jComFlag.addItem("日统计越下限时间(163)");//13
//41
    jComFlag.addItem("日电压质量越上限点数(165)");//14
    jComFlag.addItem("日电压质量越下限点数(166)");//15
    jComFlag.addItem("日电压质量越上限时间(167)");//16
    jComFlag.addItem("日电压质量越下限时间(168)");//17
    jComFlag.addItem("日电压质量合格率(169)");//18
    jComFlag.addItem("日奖励电量(170)");//19
    jComFlag.addItem("日扣罚电量(171)");//20
    jComFlag.addItem("日奖罚电量(172)");//21
    jComFlag.addItem("日计划准确率(180)");//22
//50
    jComFlag.addItem("计划上限(200)");//23
    jComFlag.addItem("计划下限(201)");//24
//52
    jComFlag.addItem("最大值相关数据");
    jComFlag.addItem("最小值相关数据");
     */

    jComhour.addItem("无效");
    for(i=1;i<25;i++){
      jComhour.addItem(Integer.toString(i));
    }

//    jComFac.setSelectedIndex(0);
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
//      System.out.println("无记录!");
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
//       System.out.println("无记录!");
        return;
      }
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_3 = new String(buffer[i]);
        str_3 = str_3.trim();
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        jComDec.addItem(str_2 + " [代码为：" +str_3 + "]");
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
//        System.out.println("无记录!");
        return;
      }
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_3 = new String(buffer[i]);
        str_3 = str_3.trim();
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        jComDec.addItem(str_2 + " [代码为：" +str_3 + "]");
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
    try{
      int m_col = usacreport.FrmSacReport.jBook1.getSelStartCol();
      int m_row = usacreport.FrmSacReport.jBook1.getSelStartRow();
      usacreport.FrmSacReport.jBook1.setActiveCell(m_row,m_col);
      String code = "@";
      String flag = "";
      boolean bfill = true;
      int SelectFlagIndex=jComFlag.getSelectedIndex();
      switch(jComtype.getSelectedIndex()){
        case 0:
          {
            if(jComFlag.getSelectedIndex() == 5 || jComFlag.getSelectedIndex()==6)
            {
              code = code + GetFlag() + "," + jTextname.getText().trim() + ",0";
              break;
            }
            if(jCheckgl.isSelected() == true){//是否规律数据
              if(jComdatabase.getSelectedIndex() == 0){//是否遥测参数
                  code = code + "dh," + jTextname.getText().trim() + ",0";
              }else{
                if(SelectFlagIndex==8)
                {
                  code = code + "db," + jTextname.getText().trim() + ",0";
                }
                else
                {
                 code = code + "dd," + jTextname.getText().trim() + ",0";
                }
              }
            }else{
              if(jComhour.getSelectedIndex() != 0){
                if(jComdatabase.getSelectedIndex() == 0){//是否遥测参数
                   if (jComFlag.getSelectedIndex() == 41)
                      code = code + "e" + GetFlag() + "," +
                          jTextname.getText().trim() + "," +
                          Integer.toString(jComhour.getSelectedIndex());
                   else
                      code = code + "dh," + jTextname.getText().trim() + "," +
                          Integer.toString(jComhour.getSelectedIndex());
                }else{
                  code = code + "dd," + jTextname.getText().trim() + "," + Integer.toString(jComhour.getSelectedIndex());
                }
              }
              else{
              if(jComFlag.getSelectedIndex() < 42){
                 if(jComFlag.getSelectedIndex() == 41)
                 {
                    JOptionPane.showMessageDialog(this, "请选择小时");
                    return;
                 }
                if(jComdatabase.getSelectedIndex() == 0){//是否遥测参数
                  code = code + "e" + GetFlag() + "," + jTextname.getText().trim() + ",0";
                }else{
                  code = code + "e" + GetFlagkwh() + "," + jTextname.getText().trim() + ",0";
                }
              }else{
                if(jComFlag.getSelectedIndex() == 36){
                  code = code + "xd," + jTextname.getText().trim() + ",0";
                }else{
                  code = code + "xh," + jTextname.getText().trim() + ",0";
                }

                usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
                String dbname = "modeldb";
                String tablename = "";

                tablename = "负荷相关参数表";
                String fieldname = "相关代码";
                String where = "",order = "";
                where = "代码 = '" + jTextname.getText().trim() + "'";

                int []icount = new int[1];
                int i = 0,j = 0;
                int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                                  order.getBytes(),icount,true);
                if(returnv <= 0){
//                  System.out.println("无相关代码!");
                  return;
                }
                byte [][] buffer = new byte[0][0];
                buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
                int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                bfill = false;
                for(i=0;i<m_ret;i++){
                  m_col ++;
                  String str_1 = new String (buffer[i]);
                  str_1 = str_1.trim();
                  code = "@" + jTextname.getText().trim() + "," + str_1 + ",0";
                  usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
                }
              }
              }
            }
            break;
          }
        case 1:
          {
            if(jCheckgl.isSelected() == true){
              if(jComFlag.getSelectedIndex() == 38){
                code = code + "31," + jTextname.getText().trim() + ",0";
                break;
              }
              else{
                if(jComdatabase.getSelectedIndex() == 0){//遥测库
                  code = code + "m" + GetFlag() + ","  + jTextname.getText().trim() + ",0";
                }else{
                  code = code + "m" + GetFlagkwh() + ","  + jTextname.getText().trim() + ",0";
                }
              }
            }else{
              if(jComFlag.getSelectedIndex() < 36){
                if(jComdatabase.getSelectedIndex() == 0){//是否遥测参数
                  code = code + "n" + GetFlag() + "," + jTextname.getText().trim() + ",0";
                }else{
                  code = code + "n" + GetFlagkwh() + "," + jTextname.getText().trim() + ",0";
                }
//                code = code + "n" + GetFlag() + ","  + jTextname.getText().trim() + ",0";
              }else{
                if(jComFlag.getSelectedIndex() == 36){
                  code = code + "xm," + jTextname.getText().trim() + ",0";
                }else if(jComFlag.getSelectedIndex() == 37){
                  code = code + "xn," + jTextname.getText().trim() + ",0";
                }
                usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
                String dbname = "modeldb";
                String tablename = "";

                tablename = "负荷相关参数表";
                String fieldname = "相关代码";
                String where = "",order = "";
                where = "代码 = '" + jTextname.getText().trim() + "'";

                int []icount = new int[1];
                int i = 0,j = 0;
                int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                    order.getBytes(),icount,true);
                if(returnv <= 0){
//                  System.out.println("无相关代码!");
                  return;
                }
                byte [][] buffer = new byte[0][0];
                buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
                int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                bfill= false;
                for(i=0;i<m_ret;i++){
                  m_col ++;
                  String str_1 = new String (buffer[i]);
                  str_1 = str_1.trim();
                  code = "@" + jTextname.getText().trim() + "," + str_1 + ",0";
                  usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
                }
              }
            }
            break;
          }
        case 2:
          {
            if(jCheckgl.isSelected() == true){//是否规律数据
              code = code + "y" + GetFlag() + ","  + jTextname.getText().trim() + ",0";
            }else{
              if(jComFlag.getSelectedIndex() < 36){
                if(jComdatabase.getSelectedIndex() == 0){//是否遥测参数
                  code = code + "z" + GetFlag() + "," + jTextname.getText().trim() + ",0";
                }else{
                  code = code + "z" + GetFlagkwh() + "," + jTextname.getText().trim() + ",0";
                }
//                code = code + "z" + GetFlag() + ","  + jTextname.getText().trim() + ",0";
              }else{
                if(jComFlag.getSelectedIndex() == 36){
                  code = code + "xy," + jTextname.getText().trim() + ",0";
                }else{
                  code = code + "xz," + jTextname.getText().trim() + ",0";
                }
                usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
                String dbname = "modeldb";
                String tablename = "";

                tablename = "负荷相关参数表";
                String fieldname = "相关代码";
                String where = "",order = "";
                where = "代码 = '" + jTextname.getText().trim() + "'";

                int []icount = new int[1];
                int i = 0,j = 0;
                int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                    order.getBytes(),icount,true);
                if(returnv <= 0){
 //                 System.out.println("无相关代码!");
                  return;
                }
                byte [][] buffer = new byte[0][0];
                buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
                int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                bfill = false;
                for(i=0;i<m_ret;i++){
                  m_col ++;
                  String str_1 = new String (buffer[i]);
                  str_1 = str_1.trim();
                  code = "@" + jTextname.getText().trim() + "," + str_1 + ",0";
                  usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
                }
              }
            }
            break;
          }
      }

      if(jCheckgl.isSelected()){
        if(jCheckDir.isSelected()){
          usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
          usacreport.FrmSacReport.jBook1.setText(m_row+1,m_col,code);
        }else{
          usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
//          usacreport.FrmSacReport.jBook1.setActiveCell(m_row,m_col+1);
          usacreport.FrmSacReport.jBook1.setText(m_row,m_col+1,code);
        }
      }else{
        if(bfill){
          usacreport.FrmSacReport.jBook1.setText(m_row,m_col,code);
        }
      }
    }catch(com.f1j.util.F1Exception  e1){
    }
    this.hide();
  }



  String GetFlag(){
    String str = "s";//*10没定义
    switch(jComFlag.getSelectedIndex())
    {
      case 1:
      {
        str = "s";
        break;
      }
      case 2:
      {
        str = "t";
        break;
      }
      case 3:
      {
        str = "u";
        break;
      }
      case 4:
      {
        str = "v";
        break;
      }
      case 5:
      {
        str = "maxtime";
        break;
      }
      case 6:
      {
        str = "mintime";
        break;
      }
      case 7:
      {
        str = "w";
        break;
      }
      case 8:
      {
        str = "x";
        break;
      }
      case 9:
      {
        str = "y";
        break;
      }
      case 10:
      {
        str = "z";
        break;
      }
      case 11:
      {
        str = "11";
        break;
      }
      case 12:
      {
        str = "12";
        break;
      }
      case 13:
      {
        str = "13";
        break;
      }
      case 14://负荷率
      {
        str = "14";
        break;
      }
      case 15:
      {
        str = "15";
        break;
      }
      case 16:
      {
        str = "16";
        break;
      }
      case 17:
      {
        str = "17";
        break;
      }
      case 18:
      {
        str = "18";
        break;
      }
      case 19:
      {
        str = "1";
        break;
      }
      case 20:
      {
        str = "2";
        break;
      }
      case 21:
      {
        str = "3";
        break;
      }
      case 22:
      {
        str = "4";
        break;
      }
      case 23:
      {
        str = "5";
        break;
      }
      case 24:
      {
        str = "6";
        break;
      }
      case 25:
      {
        str = "7";
        break;
      }
      case 26:
      {
        str = "8";
        break;
      }
      case 27:
      {
        str = "9";
        break;
      }
      case 28:
      {
        str = "d";
        break;
      }
      case 29:
      {
        str = "e";
        break;
      }
      case 30:
      {
        str = "f";
        break;
      }
      case 31:
      {
        str = "g";
        break;
      }
      case 32:
      {
        str = "h";
        break;
      }
      case 33:
      {
        str = "i";
        break;
      }
      case 34:
      {
        str = "q";
        break;
      }
      case 35:
      {
        str = "r";
        break;
      }
      case 36:
      {
        str = "xg";
        break;
      }
      case 37:
      {
        str = "xh";
        break;
      }
    case 38:
    {
      str = "31";
      break;
    }
  case 39:
  {
    str = "max";
    break;
  }
case 40:
{
  str = "min";
  break;
}
case 41:
{
  str = "a";
  break;
}

    }
    return str;
  }

  void jComdatabase_actionPerformed(ActionEvent e) {
    jComFlag.removeAllItems();
    jComDec.removeAllItems();
    switch(jComdatabase.getSelectedIndex()){
      case 0://遥测
      {
        jComFlag.addItem("整点数据");
        jComFlag.addItem("日统计最大值");//s
        jComFlag.addItem("日统计最大值发生时间");//t
        jComFlag.addItem("日统计最小值");//u
        jComFlag.addItem("日统计最小值发生时间");//v
        jComFlag.addItem("整点最大值时间");//maxtime
        jComFlag.addItem("整点最小值时间");//mintime
        jComFlag.addItem("日统计合格率");//w
        jComFlag.addItem("日统计平均值");//x
        jComFlag.addItem("日统计负荷率");//y
        jComFlag.addItem("日统计越上限点数");//z
        jComFlag.addItem("日统计越下限点数");//11
        jComFlag.addItem("日统计越上限时间");//12
        jComFlag.addItem("日统计越下限时间");//13
        jComFlag.addItem("日电压质量越上限点数");//14
        jComFlag.addItem("日电压质量越下限点数");//15
        jComFlag.addItem("日电压质量越上限时间");//16
        jComFlag.addItem("日电压质量越下限时间");//17
        jComFlag.addItem("日电压质量合格率");//18
//        jComFlag.addItem("整点最大值时间");//maxtime
//        jComFlag.addItem("整点最小值时间");//mintime
        jComFlag.addItem("实时最大值");//1
        jComFlag.addItem("实时最大值发生时间");//2
        jComFlag.addItem("实时最小值");//3
        jComFlag.addItem("实时最小值发生时间");//4
        jComFlag.addItem("实时合格率");//5
        jComFlag.addItem("实时平均值");//6
        jComFlag.addItem("实时负荷率");//7
        jComFlag.addItem("总越上上限次数");//8
        jComFlag.addItem("总越上限次数");//9
        jComFlag.addItem("总越下限次数");//d
        jComFlag.addItem("总越下下限次数");//e
        jComFlag.addItem("总越上上限时间");//f
        jComFlag.addItem("总越上限时间");//g
        jComFlag.addItem("总越下限时间");//h
        jComFlag.addItem("总越下下限时间");//i
        jComFlag.addItem("日死数时间");//q
        jComFlag.addItem("日停运时间");//r
        jComFlag.addItem("最大值相关数据");
        jComFlag.addItem("最小值相关数据");
        jComFlag.addItem("月统计每天整点数据");

        jComFlag.addItem("整点数据最大值");//max
        jComFlag.addItem("整点数据最小值");//min
        jComFlag.addItem("计划值(51)"); //a
        break;
      }
      case 1://电度
      {
        jComFlag.addItem("电度数据");
        jComFlag.addItem("1时段日电量(平)");//j
        jComFlag.addItem("2时段日电量(峰)");//k
        jComFlag.addItem("3时段日电量(谷))");//l
        jComFlag.addItem("4时段日电量(尖)");//m
        jComFlag.addItem("5时段日电量(脊骨)");//n
        jComFlag.addItem("6时段日电量(备用)");//o
        jComFlag.addItem("日累计电量");//p
        jComFlag.addItem("电度表码");
        break;
      }
    }
  }

  String GetFlagkwh(){
    String str_flag = "";
    switch(jComFlag.getSelectedIndex()){
      case 0:
        str_flag = "b";
        break;
      case 1:
        str_flag = "j";
        break;
      case 2:
        str_flag = "k";
        break;
      case 3:
        str_flag = "l";
        break;
      case 4:
        str_flag = "m";
        break;
      case 5:
        str_flag = "n";
        break;
      case 6:
        str_flag = "o";
        break;
      case 7:
        str_flag = "p";
        break;
    }
    return str_flag;
  }

  void jComtype_actionPerformed(ActionEvent e) {
    if(jComtype.getSelectedIndex() != 0){
      jComhour.setEnabled(false);
    }else{
      jComhour.setEnabled(true);
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
