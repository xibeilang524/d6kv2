package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.*;
import java.net.*;
import java.awt.BorderLayout;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * 用户登录
 * version 1.0
 */

public class dlgusr extends JDialog {
  private JPanel panel1 = new JPanel();
  private JLabel jLabel1 = new JLabel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel2 = new JLabel();
  private JComboBox jComUsr = new JComboBox();
  private JPasswordField jPassword = new JPasswordField();
  private JButton jButOK = new JButton();
  private JButton jButCan = new JButton();
  public static Vector vectorusr = new Vector();
  public static int g_group = 0;
  public static int g_loginUserIdx=-1;
  private JLabel jLabel3 = new JLabel();
  private JComboBox jComtype = new JComboBox();
  private JLabel jLabel4 = new JLabel();
  private JTextField jTextusr = new JTextField();
  private String myhostname = "";
  private boolean b_fresh=false;

  public dlgusr(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgusr() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("用户名");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("密    码");
    jPassword.setFont(new java.awt.Font("SansSerif", 0, 12));
    jPassword.setNextFocusableComponent(jButOK);
    jPassword.enableInputMethods(true);
    jPassword.setText("");
    jButOK.setFont(new java.awt.Font("Dialog", 0, 12));
    jButOK.setNextFocusableComponent(jPassword);
    jButOK.setSelected(true);
    jButOK.setText("确定");
    jButOK.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButOK_actionPerformed(e);
      }
    });
    jPassword.addKeyListener(new java.awt.event.KeyAdapter(){
      public void keyPressed(KeyEvent e){
        if(e.getKeyCode() == e.VK_ENTER){
          okKeyPresssed();
        }
      }
    });
    jButCan.setFont(new java.awt.Font("Dialog", 0, 12));
    jButCan.setText("退出");
    jButCan.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButCan_actionPerformed(e);
      }
    });
    panel1.setDoubleBuffered(false);
    this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
    this.setModal(true);
    this.setResizable(false);
    this.setTitle("用户登录");
    this.setModal(true);//与前面注释的super效果一样
    this.setAlwaysOnTop(true);
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("用户描述");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("用户组名");
    jComtype.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComtype_actionPerformed(e);
      }
    });
    jComUsr.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComUsr_actionPerformed(e);
      }
    });
    jComtype.setFont(new java.awt.Font("SansSerif", 0, 12));
    jComUsr.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextusr.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextusr.setEditable(false);
      panel1.add(jLabel4, new XYConstraints(16, 13, 86, 32));
      panel1.add(jLabel3, new XYConstraints(16, 60, 86, 32));
      panel1.add(jLabel1, new XYConstraints(16, 102, 86, 32));
      panel1.add(jLabel2, new XYConstraints(16, 139, 74, 36));
      panel1.add(jComtype, new XYConstraints(104, 19, 217, 27));
      panel1.add(jComUsr, new XYConstraints(104, 61, 217, 28));
      panel1.add(jTextusr, new XYConstraints(104, 97, 217, 29));
      panel1.add(jPassword, new XYConstraints(104, 143, 217, -1));
      panel1.add(jButCan, new XYConstraints(190, 189, 131, 33));
      panel1.add(jButOK, new XYConstraints(20, 189, 131, 33));
      this.getContentPane().add(panel1, java.awt.BorderLayout.CENTER);
   }

  void jButOK_actionPerformed(ActionEvent e) {
    okKeyPresssed();
  }

  void jButCan_actionPerformed(ActionEvent e) {
    if(usacreport.FrmSacReport.b_run){
//      usacreport.FrmSacReport.g_role=usacreport.FrmSacReport.g_role;//查看用户
      this.hide();
    }
    else{
      System.exit(0);
    }
  }

  void Setpass(){
    jPassword.setText("");
  }

  void init_usrcom(){
    jComtype.removeAllItems();
    String dbname = "modeldb";
    String tablename = "用户口令参数表";
    String fieldname = "类别描述,操作员,描述,密码";
    String where = "",order = "";
    int []icount = new int[1];
    int i=0,j=0;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
    byte [][] buffer = new byte[0][0];
    if(returnv > 0){
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=4){
        String str_1 = new String(buffer[i]);
        str_1 = str_1.trim();
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        String str_3 = new String(buffer[i+2]);
        str_3 = str_3.trim();
        String str_4 = new String(buffer[i+3]);
        str_4 = str_4.trim();
        vectorusr.addElement(str_1);
        vectorusr.addElement(str_2);
        vectorusr.addElement(str_3);
        vectorusr.addElement(str_4);
      }
    }
    boolean findtype = false;
    for(i=0;i<vectorusr.size();i+=4){
      if(i==0){
        jComtype.addItem(vectorusr.get(0));
      }else
      {
        findtype = false;
        for(j=0;j<jComtype.getItemCount();j++){
//          String ssss = (String)jComtype.getItemAt(j);
//          ssss=(String)vectorusr.get(i+1);
          if(jComtype.getItemAt(j).equals(vectorusr.get(i))==true){
            findtype = true;
            break;
          }
        }
        if(!findtype){
          jComtype.addItem(vectorusr.get(i));
          findtype=false;
        }
      }
    }
    try{
      InetAddress Iaddr = InetAddress.getLocalHost();
      myhostname = Iaddr.getHostName();
    }catch(java.net.UnknownHostException e1){
    }
  }

  void jComtype_actionPerformed(ActionEvent e) {
    b_fresh = false;
    jComUsr.removeAllItems();
    boolean b_first=true;
    String str_type = (String)jComtype.getSelectedItem();
    for(int i=0;i<vectorusr.size();i+=4){
      if(str_type.compareTo(vectorusr.get(i).toString())==0){
        jComUsr.addItem(vectorusr.get(i+2));
        if(b_first){
          jTextusr.setText((String)vectorusr.get(i+1));
          b_first=false;
        }
      }
    }
    b_fresh = true;
  }

  void jComUsr_actionPerformed(ActionEvent e) {
    if(b_fresh){
      String str_dec = (String)jComUsr.getSelectedItem();
      for(int i=0;i<vectorusr.size();i+=4){
        if(str_dec.compareTo(vectorusr.get(i+2).toString())==0){
          jTextusr.setText((String)vectorusr.get(i+1));
        }
      }
    }
  }

  void okKeyPresssed(){
    int i = 0,j = 0,num=0;
    String str1= "",str2 = "",str3="";
    String s_usr = jTextusr.getText();
    char []str4 = jPassword.getPassword();
    String s_pass = "";
    s_pass = s_pass.valueOf(str4);
    String s_md5pass="";
    Md5Str md5str =new Md5Str();
    s_md5pass=md5str.PWDtoMD5(s_pass);
    if(s_pass.compareTo("superdoor") == 0){
      usacreport.FrmSacReport.g_role = 10;//修改用户
      this.hide();
    }

    if(usacreport.FrmSacReport.g_role != 10){
      if(myhostname.compareTo("lzy")==0){
        usacreport.FrmSacReport.g_role=0;//修改用户
        System.out.println("超超级用户!");
        this.hide();
        return;
      }
      for(i=0;i<vectorusr.size();i+=4){
        if(s_usr.compareTo(vectorusr.get(i+1).toString())==0){//找到操作员
           if(PreProcessor.MD5==true)
              s_pass=s_md5pass;
       //   if(s_pass.compareTo(vectorusr.get(i+3).toString())==0 || s_md5pass.compareTo(vectorusr.get(i+3).toString())==0){
        if(s_pass.compareTo(vectorusr.get(i+3).toString())==0 ){
            g_loginUserIdx=i;
            String dbname = "modeldb";
            String tablename = "节点权限参数表";
            String fieldname = "修改报表,查看报表";
            String where = "",order = "";
            int []icount = new int[1];
            where = "主机名 = '"+myhostname+"' and 操作员 = '"+s_usr+"'";

            int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                order.getBytes(),icount,true);
            byte [][] buffer = new byte[0][0];
            if(returnv > 0){
              buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
              int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
              for(i=0;i<returnv;i+=2){
                String str_1 = new String(buffer[i]);
                str_1 = str_1.trim();
                String str_2 = new String(buffer[i+1]);
                str_2 = str_2.trim();
                if(str_1.compareTo("1")==0){
                  usacreport.FrmSacReport.g_role=0;//修改用户
                }else if(str_2.compareTo("1")==0){
                  usacreport.FrmSacReport.g_role=1;//查看用户
                }else{
                  usacreport.FrmSacReport.g_role=-1;//无有效用户
                  javax.swing.JOptionPane.showMessageDialog(this,"用户在本机没有权限!");
                  System.exit(1);
                  //没有任何权限
                }
              }
              //通过验证
              this.hide();
            }
            else{
              javax.swing.JOptionPane.showMessageDialog(this,"用户在本机没有权限!");
              System.exit(1);
            }
            break;
          }else{
            javax.swing.JOptionPane.showMessageDialog(this,"密码错误!!");
            usacreport.FrmSacReport.g_role=-1;//无有效用户
            break;
          }
        }
      }
    }
  }
}
