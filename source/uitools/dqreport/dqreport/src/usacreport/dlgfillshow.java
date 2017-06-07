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

public class dlgfillshow extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JLabel jLabel2 = new JLabel();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JTextField jTextdec = new JTextField();
  private JTextField jTextname = new JTextField();
  private JTextField jTextFlag = new JTextField();
  private JTextField jTexthour = new JTextField();
  private JCheckBox jCheckgl = new JCheckBox();
  private JButton jButOK = new JButton();

  public dlgfillshow(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgfillshow() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("描述");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("记录名");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("FLAG");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("小时参数");
    jCheckgl.setFont(new java.awt.Font("Dialog", 0, 12));
    jCheckgl.setText("是否规律数据");
    jButOK.setFont(new java.awt.Font("Dialog", 0, 12));
    jButOK.setText("退出");
    jButOK.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButOK_actionPerformed(e);
      }
    });
    jTextdec.setFont(new java.awt.Font("Dialog", 0, 12));
    jTexthour.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextFlag.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextname.setFont(new java.awt.Font("Dialog", 0, 12));
    getContentPane().add(panel1);
    panel1.setBorder(new EmptyBorder(5, 5, 15, 15));
    panel1.add(jLabel1, new XYConstraints(15, 16, 94, 25));
    panel1.add(jLabel2, new XYConstraints(15, 57, 94, 25));
    panel1.add(jLabel3, new XYConstraints(14, 99, 94, 25));
    panel1.add(jLabel4, new XYConstraints(13, 144, 94, 25));
    panel1.add(jTextdec,     new XYConstraints(131, 17, 196, 30));
    panel1.add(jTexthour,          new XYConstraints(131, 143, 196, 30));
    panel1.add(jTextFlag,        new XYConstraints(131, 101, 196, 30));
    panel1.add(jTextname,        new XYConstraints(131, 59, 196, 30));
    panel1.add(jCheckgl,      new XYConstraints(11, 194, 142, 32));
      panel1.add(jButOK, new XYConstraints(229, 213, 98, 28));
/**    try{
     int m_row = usacreport.FrmSacReport.jBook1.getRow();
      int m_col =  usacreport.FrmSacReport.jBook1.getCol();
      usacreport.FrmSacReport.jBook1.setActiveCell(m_row,m_col);
      String s_str = usacreport.FrmSacReport.jBook1.getText();
      if(s_str.length() == 0) return;
      if(s_str.substring(0,1).compareTo("@") == 0){
        System.out.println("无效字符串");
        return ;
      }
      String str="",str1="",str2="";
      str = s_str.substring(1,3);
      int pos = s_str.lastIndexOf(",");
      str1 = s_str.substring(4,pos);
      str2 = s_str.substring(pos+1,s_str.length());

      String dbname = "modeldb";
      String tablename = "";
      String fieldname = "描述";
      String where = "代码 = '" + str1 + "'",order = "";
      jTextname.setText(str1);

      String str3 = str.substring(1,2),m_flag = "";
      if(str3.compareTo("1") ==0) m_flag = "最大值";
      else if(str3.compareTo("2") ==0) m_flag = "最大值发生时间";
      else if(str3.compareTo("3") ==0) m_flag = "最小值";
      else if(str3.compareTo("4") ==0) m_flag = "最小值发生时间";
      else if(str3.compareTo("5") ==0) m_flag = "合格率";
      else if(str3.compareTo("6") ==0) m_flag = "平均值";
      else if(str3.compareTo("7") ==0) m_flag = "日死数时间";
      else if(str3.compareTo("8") ==0) m_flag = "日停运时间";
      else if(str3.compareTo("9") ==0) m_flag = "日统计最大值";
      else if(str3.compareTo("a") ==0) m_flag = "日统计最小值";
      else if(str3.compareTo("b") ==0) m_flag = "日统计合格率";
      else if(str3.compareTo("c") ==0) m_flag = "日统计平均值";
      else if(str3.compareTo("d") ==0) m_flag = "越上限次数";
      else if(str3.compareTo("f") ==0) m_flag = "越下限次数";
      else if(str3.compareTo("g") ==0) m_flag = "日累计全网电量";
      else if((str3.compareTo("h") ==0) && (str3.compareTo("d") != 0)) m_flag = "1时段日电量(平)";
      else if((str3.compareTo("h") ==0) && (str3.compareTo("d") == 0)) m_flag = "整数据";
      else if(str3.compareTo("i") ==0)  m_flag = "2时段日电量(峰)";
      else if(str3.compareTo("j") ==0)  m_flag = "3时段日电量(谷)";
      else if(str3.compareTo("k") ==0)  m_flag = "4时段日电量(尖)";
      else if(str3.compareTo("l") ==0)  m_flag = "5时段日电量(脊骨)";
      else if(str3.compareTo("m") ==0) m_flag = "6时段日电量";
      else if(str3.compareTo("n") ==0)  m_flag = "日统计最大值发生时间";
      else if(str3.compareTo("o") ==0) m_flag = "日统计最小值发生时间";
      else if(str3.compareTo("p") ==0)  m_flag = "负荷率";

      jTextFlag.setText(m_flag);
      jTexthour.setText(str2);

      if(str3.compareTo("g") == 0 || str3.compareTo("h") == 0 ||str3.compareTo("i") == 0 ||
         str3.compareTo("j") == 0 ||str3.compareTo("k") == 0 ||str3.compareTo("l") == 0 ||str3.compareTo("m") == 0){
           tablename = "电度参数表";
         }
         else{
           tablename = "遥测参数表";
         }
      if(str.substring(0,2).compareTo("dh") == 0){
        tablename = "遥测参数表";
        jCheckgl.setSelected(true);
      }else if(str.substring(0,2).compareTo("dd") == 0){
        tablename = "电度参数表";
        jCheckgl.setSelected(true);
      }

      int []icount = new int[1];
      int i = 0,j = 0;
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
      if(returnv <= 0){
        jTextdec.setText("无效代码");
        System.out.println("无记录!");
        return;
      }
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      String str_dec = new String(buffer[0]);
      str_dec = str_dec.trim();
      jTextdec.setText(str_dec);
    }catch(com.f1j.util.F1Exception  e){
    }*/
  }

  void jButOK_actionPerformed(ActionEvent e) {
    this.hide();
  }

  void Freshtable(){
    int hour_para = 0;
    jCheckgl.setSelected(false);
    usacreport.cellpara1 para_temp = new usacreport.cellpara1();
    try{
     int m_row = usacreport.FrmSacReport.jBook1.getRow();
      int m_col =  usacreport.FrmSacReport.jBook1.getCol();
      String s_str = "";
      if(usacreport.dlgSelect.veccontent.isEmpty()==true)
      {
        s_str = usacreport.FrmSacReport.jBook1.getText(m_row,m_col);
      }
      else
      {
        for(int kk = 0;kk<usacreport.dlgSelect.veccontent.size();kk++)
        {
          para_temp = (usacreport.cellpara1)usacreport.dlgSelect.veccontent.get(kk);
          if(m_row == para_temp.row && m_col == para_temp.col)
          {
            s_str = para_temp.str_cell;
            break;
          }
        }
      }

      if(s_str.length() == 0) return;

      //前缀@
      if(s_str.substring(0,1).compareTo("@") != 0)
      {
        jTextdec.setText("");
        jTextFlag.setText("");
        jTexthour.setText("");
        jTextname.setText("");
        jCheckgl.setSelected(true);
        return ;
      }

      String str="",str1="",str2="",str_temp="";

      //特殊定义
      if(s_str.substring(1,s_str.length()) == "maxtime")
      {
        jTextFlag.setText("整点最大值发生时间");
        return;
      }else if(s_str.substring(1,s_str.length()) == "mintime"){
        jTextFlag.setText("整点最小值发生时间");
        return;
      }

      int pos = 0;
      String m_flag = "";
      pos = s_str.indexOf(',');

      //获取类型
      str = s_str.substring(1,pos);//type
      str_temp = s_str.substring(pos+1,s_str.length());

      //获取代码
      pos = str_temp.indexOf(",");
      str1 = str_temp.substring(0,pos);//name

      //获取时间
      str2 = str_temp.substring(pos+1,str_temp.length());//hour
      hour_para = Integer.parseInt(str2);

      String dbname = "modeldb";
      String tablename = "";
      String fieldname = "描述";
      String where = "代码 = '" + str1 + "'",order = "";
      jTextname.setText(str1);

      if(str.compareTo("xg") != 0){
        String str3 = str.substring(1,str.length());//flag
        String str4 = str.substring(0,1);//type
        if(str.compareTo("dh")==0){
          m_flag = "YC整点数据(<50)";
        }else if(str.compareTo("dd")==0){
          m_flag = "电度整点数据(<50)";
        }else if(str.compareTo("31")==0){
          m_flag = "月统计每天整点数据";
        }else if(str.compareTo("maxtime")==0){
          m_flag = "整点最大值发生时间";
        }else if(str.compareTo("mintime")==0){
          m_flag = "整点最小值发生时间";
        }
        else if(str3.compareTo("a") ==0) m_flag = "YC计划值(51)";
        else if(str3.compareTo("b") ==0) m_flag = "电度表码(52)";
        else if(str3.compareTo("c") ==0) m_flag = "电度原始数据(53)";

        else if(str3.compareTo("1") ==0) m_flag = "实时最大值(55)";
        else if(str3.compareTo("2") ==0) m_flag = "实时最大值发生时间(55)";
        else if(str3.compareTo("3") ==0) m_flag = "实时最小值(56)";
        else if(str3.compareTo("4") ==0) m_flag = "实时最小值发生时间(56)";
        else if(str3.compareTo("5") ==0) m_flag = "实时合格率(57)";
        else if(str3.compareTo("6") ==0) m_flag = "实时平均值(58)";
        else if(str3.compareTo("7") ==0) m_flag = "实时负荷率(59)";

        else if(str3.compareTo("8") ==0) m_flag = "越上上限次数(62)";
        else if(str3.compareTo("9") ==0) m_flag = "越上限次数(63)";
        else if(str3.compareTo("d") ==0) m_flag = "越下限次数(64)";
        else if(str3.compareTo("e") ==0) m_flag = "越下限次数(65)";
        else if(str3.compareTo("f") ==0) m_flag = "越上上限时间(66)";
        else if(str3.compareTo("g") ==0) m_flag = "越上限时间(67)";
        else if(str3.compareTo("h") ==0) m_flag = "越下限时间(68)";
        else if(str3.compareTo("i") ==0) m_flag = "越下下限时间(69)";
        else if(str3.compareTo("j") ==0) m_flag = "时段日电量(平)(70)";
        else if(str3.compareTo("k") ==0) m_flag = "时段日电量(峰)(71)";
        else if(str3.compareTo("l") ==0) m_flag = "时段日电量(谷)(72)";
        else if(str3.compareTo("m") ==0) m_flag = "时段日电量(尖)(73)";
        else if(str3.compareTo("n") ==0) m_flag = "时段日电量(脊骨)(74)";
        else if(str3.compareTo("o") ==0) m_flag = "时段日电量(备用)(75)";
        else if(str3.compareTo("p") ==0) m_flag = "日累计电量(79)";
        else if(str3.compareTo("q") ==0) m_flag = "日死数时间(80)";
        else if(str3.compareTo("r") ==0) m_flag = "日停运时间(81)";

        else if(str3.compareTo("s") ==0) m_flag = "日统计最大值(150)";
        else if(str3.compareTo("t") ==0) m_flag = "日统计最大值发生时间(150)";
        else if(str3.compareTo("u") ==0) m_flag = "日统计最小值(151)";
        else if(str3.compareTo("v") ==0) m_flag = "日统计最小值发生时间(151)";
        else if(str3.compareTo("w") ==0) m_flag = "日统计合格率(152)";
        else if(str3.compareTo("x") ==0) m_flag = "日统计平均值(153)";
        else if(str3.compareTo("y") ==0) m_flag = "日统计负荷率(154)";

        else if(str3.compareTo("z") ==0) m_flag = "日统计越上限点数(158)";
        else if(str3.compareTo("11") ==0) m_flag = "日统计越下限点数(159)";
        else if(str3.compareTo("12") ==0) m_flag = "日统计越上限时间(162)";
        else if(str3.compareTo("13") ==0) m_flag = "日统计越下限时间(163)";

        else if(str3.compareTo("14") ==0) m_flag = "日电压质量越上限点数(165)";
        else if(str3.compareTo("15") ==0) m_flag = "日电压质量越下限点数(166)";
        else if(str3.compareTo("16") ==0) m_flag = "日电压质量越上限时间(167)";
        else if(str3.compareTo("17") ==0) m_flag = "日电压质量越下限时间(168)";
        else if(str3.compareTo("18") ==0) m_flag = "日电压质量合格率(169)";
        else if(str3.compareTo("19") ==0) m_flag = "日奖励电量(170)";
        else if(str3.compareTo("20") ==0) m_flag = "日扣罚电量(171)";
        else if(str3.compareTo("21") ==0) m_flag = "日奖罚电量(172)";
        else if(str3.compareTo("22") ==0) m_flag = "日计划准确率(180)";

        else if(str3.compareTo("23") ==0) m_flag = "计划上限(200)";
        else if(str3.compareTo("24") ==0) m_flag = "计划下限(201)";
        else {return;}

        jTextFlag.setText(m_flag);
        jTexthour.setText(str2);

        if(str3.compareTo("g") == 0 || str3.compareTo("h") == 0 ||str3.compareTo("i") == 0 ||
           str3.compareTo("j") == 0 ||str3.compareTo("k") == 0 ||str3.compareTo("l") == 0 ||str3.compareTo("m") == 0){
             tablename = "电度参数表";
           }
           else{
             tablename = "遥测参数表";
           }
        if(str.substring(0,2).compareTo("dh") == 0){
          tablename = "遥测参数表";
          jCheckgl.setSelected(true);
        }else if(str.substring(0,2).compareTo("dd") == 0){
          tablename = "电度参数表";
          jCheckgl.setSelected(true);
        }
      }else{
        jCheckgl.setSelected(false);
        tablename = "遥测参数表";
        m_flag = "相关数据类型";
        jTextFlag.setText(m_flag);
        jTexthour.setText(str2);
      }

      int []icount = new int[1];
      int i = 0,j = 0;
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
      if(returnv <= 0){
        jTextdec.setText("无效代码");
//        System.out.println("无记录!");
        return;
      }
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      String str_dec = new String(buffer[0]);
      str_dec = str_dec.trim();
      jTextdec.setText(str_dec);
    }catch(com.f1j.util.F1Exception  e1){
    }

    if(hour_para>0){
      jTextFlag.setText("整点数据");
      return;
    }
  }
}
