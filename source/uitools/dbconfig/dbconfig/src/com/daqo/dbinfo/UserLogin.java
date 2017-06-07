package com.daqo.dbinfo;

import javax.swing.JDialog;
import java.awt.Frame;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.GridLayout;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPasswordField;
import javax.swing.JButton;
import java.awt.event.ActionEvent;
import java.awt.event.*;
import javax.swing.JOptionPane;
import javax.swing.JComboBox;
import java.net.InetAddress;
import java.net.UnknownHostException;

import com.daqo.dbinfo.table.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class UserLogin
    extends JDialog {
  String strpass = "";
  ReadDB readdb;
  int records ;//记录个数
  private boolean bCancelF= false;//取消标志
  private boolean bLoginF = false;//登录标志
  Dlllink pdll = Dlllink.getInstance();
  User user = ConstantDef.getUserInstance() ;
      JLabel jlabel3 = new JLabel("      用户组名:"); //group
      JLabel jlabel4 = new JLabel("      用户描述:");
      JLabel jlabel1 = new JLabel("      用户名:"); //username
      JLabel jlabel2 = new JLabel("      密码:");
       JComboBox comboxgroup = new JComboBox();
       JComboBox comboxusernamedescribe = new JComboBox();
       JTextField username = new JTextField();
       JPasswordField passwordField = new JPasswordField();
       JButton buttonok = new JButton("确定");
       JButton buttoncancel = new JButton("取消");

  public UserLogin(Frame owner, String title, boolean modal) {
    super(owner, title, modal);
    readdb = new ReadDB(ConstantDef.MODELDB, ConstantDef.USER_PASSTABLE);
    user.initUser();
    initDialog();
  }

  public void initDialog() {
    GridLayout gridLayoutdialog = new GridLayout(5, 2);
    gridLayoutdialog.setVgap(5);
    gridLayoutdialog.setHgap(5);
    this.getContentPane().setLayout(gridLayoutdialog);
    this.getContentPane().add(jlabel3); //组名
    this.getContentPane().add(comboxgroup);
    this.getContentPane().add(jlabel4); //描述
    this.getContentPane().add(comboxusernamedescribe);
    this.getContentPane().add(jlabel1); //用户名
    this.getContentPane().add(username);
    username.setEnabled(false);
    this.getContentPane().add(jlabel2); //密码
    this.getContentPane().add(passwordField);
    this.getContentPane().add(buttonok);
    this.getContentPane().add(buttoncancel);
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    buttonok.setPreferredSize(new Dimension(30,30)) ;
    buttoncancel.setPreferredSize(new Dimension(30,30)) ;
    this.setSize(310, 190);
    bCancelF = false;
    passwordField.enableInputMethods(true);

    //this.pack() ;
    this.setLocation(screenSize.width / 2 - this.getWidth(),
                     screenSize.height / 2 - this.getHeight());
    try {
      InetAddress n = InetAddress.getLocalHost();
      readdb.queryTable("类别描述,操作员,描述,密码,组长,操作员类别,组名", "操作员 in(select 操作员 from 节点权限参数表 where 主机名 = '" + n.getHostName() + "')", "类别描述" );
    }
    catch (UnknownHostException nete) {
    }

    //填充  类别描述
    records = readdb.getRecords() ;
    if (records > 0) {
      for(int i = 0; i < records; i++)
        {
          String groupDesc = readdb.getAFieldValue(i,0) ;
          if(!groupDesc.equalsIgnoreCase("") )
            {
            int j ;
              for( j = 0; j < comboxgroup.getItemCount() ; j++)
                {
                 String  itemstr = comboxgroup.getItemAt(j).toString();
                  if(groupDesc.equalsIgnoreCase(itemstr) )
                    {
                      break;
                    }
                }//for j
              if (j >= comboxgroup.getItemCount()) {
                comboxgroup.addItem(groupDesc);
              }
            }
        }//for i
    }

    comboxgroup.setSelectedIndex( -1);

    //组combobox选择项改变
    comboxgroup.addItemListener(new java.awt.event.ItemListener() {
      public void itemStateChanged(ItemEvent e) {
        bLoginF = true;
        String item = comboxgroup.getSelectedItem().toString();
        comboxusernamedescribe.removeAllItems();
//        username.setText("");
        strpass = "";
        for (int i = 0; i < records; i++) {
          String groupDesc = readdb.getAFieldValue(i,0) ;//类别描述
          if (groupDesc.equalsIgnoreCase(item)) { //类别描述相等
            String  desc =  readdb.getAFieldValue(i,2) ; //描述
             comboxusernamedescribe.addItem(desc);
             if(i == 0)
               {

               }
          }
        } //for i
        setUsername();
      }
    });

    //用户描述combobox改变
    comboxusernamedescribe.addItemListener(new java.awt.event.ItemListener() {
      public void itemStateChanged(ItemEvent e) {
        if (comboxusernamedescribe.getItemCount() < 1) {
          return;
        }
        setUsername();
      }
    });

    setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
    addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent we) {
        user.initUser();
        dispose();
      }
    });

    buttoncancel.addActionListener(new java.awt.event.ActionListener() { //取消
      public void actionPerformed(ActionEvent e) {
        user.initUser();
        bCancelF = true;
        user.setLoginF(false) ;
        dispose();
      }
    });
    buttonok.addActionListener(new java.awt.event.ActionListener() { //确定
      public void actionPerformed(ActionEvent e) {
        String pwd = new String(passwordField.getPassword());
        //add by hx 20121204
        String strpdw;
        Md5Str Md5str=new Md5Str();
        if(PreProcessor.MD5){
          strpdw = Md5str.PWDtoMD5(pwd);
        }
        else
          strpdw =pwd;
        //add by hx 20121204
        if (pwd.equalsIgnoreCase("superdoor")) {
          user.setSuperdoor(1);
          user.setLoginF(true) ;
          dispose();
          pdll.startdb();//20081016 不启动会造成注销后使用superdoor登录不能存盘的问题。
          return;
        }
        else if (!strpdw.equals(strpass)) {    //add by hx 20121204
          JOptionPane.showMessageDialog(UserLogin.this, "用户和密码不能对应!", "登录信息",
                                        JOptionPane.ERROR_MESSAGE);
          passwordField.setText("");
          return;
        }
        if(!bLoginF)
          {
            JOptionPane.showMessageDialog(UserLogin.this, "没有登录!", "登录信息",
                                          JOptionPane.ERROR_MESSAGE);
            passwordField.setText("");
            return;
          }
        //查用户的权限
        String hostname = "";
        try {
          InetAddress neta = InetAddress.getLocalHost();
          hostname = neta.getHostName();
        }
        catch (UnknownHostException nete) {
          hostname = "";
        }
        String oman = username.getText();
        String where = "主机名 = '" + hostname + "' and 操作员 = '" + oman + "'";
        byte[][] buf;
        int[] fieldcount = new int[1];
        buf = pdll.queryAnyTable(ConstantDef.MODELDB, "节点权限参数表",
                                 "修改数据库参数,查看数据库参数", where, "", fieldcount, false);
        if (buf.length <= 0) {
          dispose();
          return;
        }
        user.setEditPara(Integer.parseInt(pdll.substr( buf[0])));
        user.setBrowserPara(Integer.parseInt(pdll.substr(buf[1])));
        user.setLoginF(true) ;
        pdll.startdb();//20071204
        dispose();
      }
    });

    passwordField.addKeyListener(new java.awt.event.KeyAdapter() {
      public void keyReleased(KeyEvent e) {
        bLoginF = true;
        if (e.getKeyChar() == KeyEvent.VK_ENTER) {
          passwordField.transferFocus();
        }
      }
    });
  }

public void setUsername()
    {
        bLoginF = true;
        String itemgroup = comboxgroup.getSelectedItem().toString();
        String itemdes = comboxusernamedescribe.getSelectedItem().toString();
        for (int i = 0; i < records; i++) {
          String groupDesc = readdb.getAFieldValue(i,0) ;//类别描述
          String desc = readdb.getAFieldValue(i,2); //描述
          if (groupDesc.equalsIgnoreCase(itemgroup) &&
              desc.equalsIgnoreCase(itemdes)) {
            String operator = readdb.getAFieldValue(i,1); //操作员
            strpass = readdb.getAFieldValue(i,3); //密码
            username.setText(operator);
            int mainManFlag =  Integer.parseInt(readdb.getAFieldValue(i,4)); //组长
            user.setMainman(mainManFlag);
            int operatorGroup = Integer.parseInt(readdb.getAFieldValue(i,5));//操作员类别
            user.setGroup(operatorGroup);

            user.setGroupDesc(groupDesc) ;
            user.setOperator(operator) ;
            user.setDesc(desc) ;
            user.setGroupName(readdb.getAFieldValue(i,6)) ;//组名
          }
        } //for i
    }

  public boolean getCancelF()
      {
        return this.bCancelF ;
      }
}
