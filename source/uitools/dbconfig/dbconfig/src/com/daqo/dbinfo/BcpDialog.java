package com.daqo.dbinfo;

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.File;
import java.io.IOException;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class BcpDialog
    extends JDialog {
  JPanel panel1 = new JPanel();
  JLabel jLabelFilename = new JLabel();
  GridLayout gridLayout1 = new GridLayout();
  JTextField jTextFieldFilename = new JTextField();
  JButton jButtonCalcel = new JButton();
  JLabel jLabelUsername = new JLabel();
  JTextField jTextFieldUsername = new JTextField();
  JPanel jPanel1 = new JPanel();
  JPanel jPanel2 = new JPanel();
  JPanel jPanel3 = new JPanel();
  JButton jButtonMake = new JButton();
  JPanel jPanel4 = new JPanel();
  JPanel jPanel5 = new JPanel();
  JLabel jLabelPass = new JLabel();
  JLabel jLabelDB = new JLabel();
  JTextField jTextFieldDB = new JTextField();
  JRadioButton jRadioButton1 = new JRadioButton();
  JPasswordField jFieldPassword = new JPasswordField();
  JPanel jPanel6 = new JPanel();
  JLabel jLabelSrvName = new JLabel();
  JTextField jTextFieldSrvname = new JTextField();

  public BcpDialog(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  public BcpDialog() {
    this(null, "", false);
  }

  void jbInit() throws Exception {
    panel1.setLayout(gridLayout1);
    jLabelFilename.setText("文件名:");
    jTextFieldFilename.setPreferredSize(new Dimension(120, 22));
    jTextFieldFilename.setToolTipText("");
    jTextFieldFilename.setText(Dlllink.runhome + "\\bcpout.bat");
    gridLayout1.setRows(6);
    gridLayout1.setColumns(1);
    jButtonCalcel.setText("退出");
    jButtonCalcel.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButtonCalcel_actionPerformed(e);
      }
    });
    jLabelUsername.setText("用户名:");
    jTextFieldUsername.setPreferredSize(new Dimension(120, 22));
    jTextFieldUsername.setText("sa");
    jButtonMake.setText("生成文件");
    jButtonMake.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButtonMake_actionPerformed(e);
      }
    });
    jLabelPass.setToolTipText("");
    jLabelPass.setText("密  码:");
    jLabelDB.setText("数据库:");
    jTextFieldDB.setPreferredSize(new Dimension(120, 22));
    jTextFieldDB.setText("modeldb");
    jRadioButton1.setText("out");
    jRadioButton1.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jRadioButton1_actionPerformed(e);
      }
    });
    jLabelSrvName.setText("服务器:");
    jTextFieldSrvname.setPreferredSize(new Dimension(120, 22));
    jTextFieldSrvname.setText("nbdb");
    jFieldPassword.setMinimumSize(new Dimension(94, 22));
    jFieldPassword.setPreferredSize(new Dimension(120, 22));
    this.setModal(true);
    this.setTitle("");
    panel1.setPreferredSize(new Dimension(184, 174));
    jPanel4.add(jLabelPass, null);
    jPanel4.add(jFieldPassword, null);
    getContentPane().add(panel1);
    panel1.add(jPanel1, null);
    panel1.add(jPanel2, null);
    panel1.add(jPanel5, null);
    panel1.add(jPanel4, null);
    panel1.add(jPanel6, null);
    panel1.add(jPanel3, null);
    jPanel6.add(jLabelSrvName, null);
    jPanel6.add(jTextFieldSrvname, null);
    jPanel1.add(jLabelFilename, null);
    jPanel1.add(jTextFieldFilename, null);
    jPanel2.add(jLabelUsername, null);
    jPanel2.add(jTextFieldUsername, null);
    jPanel3.add(jRadioButton1, null);
    jPanel3.add(jButtonMake, null);
    jPanel3.add(jButtonCalcel, null);
    jPanel5.add(jLabelDB, null);
    jPanel5.add(jTextFieldDB, null);
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    this.pack();
    this.setLocation(screenSize.width / 2 - this.getWidth(),
                     screenSize.height / 2 - this.getHeight());

  }

  void jButtonCalcel_actionPerformed(ActionEvent e) {
    this.dispose();
  }

  void jRadioButton1_actionPerformed(ActionEvent e) {
    if (jRadioButton1.isSelected()) {
      jRadioButton1.setText("in");
      jTextFieldFilename.setText(Dlllink.runhome + "\\bcpin.bat");
    }
    else {
      jRadioButton1.setText("out");
      jTextFieldFilename.setText(Dlllink.runhome + "\\bcpout.bat");
    }
  }

  void jButtonMake_actionPerformed(ActionEvent e) {
    String pass = new String(jFieldPassword.getPassword());
    String objfile = jTextFieldFilename.getText().trim();
    File fileobject = new File(objfile);
    String fn = fileobject.getPath() + fileobject.getName();
    jTextFieldFilename.setText(fn);
    byte[] result = Dlllink.getInstance().makeBcp(jTextFieldDB.getText().trim().
                                                  getBytes(),
                                                  jTextFieldSrvname.getText().
                                                  trim().getBytes(),
                                                  fn.trim().getBytes(),
                                                  jTextFieldUsername.getText().
                                                  trim().getBytes(),
                                                  pass.getBytes(),
                                                  jRadioButton1.isSelected()); //生成bcp文件
    if (result.length < 1) {
      JOptionPane.showMessageDialog(BcpDialog.this, "生成文件失败！", "生成文件信息",
                                    JOptionPane.ERROR_MESSAGE);
    }
    else {
      String msg = "生成了\"" + new String(result) + "\"文件";
      JOptionPane.showMessageDialog(BcpDialog.this, msg, "生成文件信息",
                                    JOptionPane.CLOSED_OPTION);
    }
  }
}
