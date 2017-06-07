package com.daqo.dbinfo;

import java.awt.*;
import java.awt.event.*;
import java.lang.Thread;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import javax.swing.Timer;
import java.lang.Class;
import java.net.InetAddress;
import java.net.UnknownHostException;

import com.daqo.dbinfo.table.*;
import java.beans.*;
import java.awt.image.BufferedImage;
import javax.swing.BorderFactory;
import java.awt.Color;
import javax.swing.border.EmptyBorder;
import java.awt.Font;
import java.io.File;
import java.io.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class FrameDB
    extends JFrame {

 public Vector VTableUpdate =new Vector(0);   //修改的表数组  ---hx

  JPanel contentPane;
  JMenuBar jMenuBar1 = new JMenuBar();
  JMenu jMenuFile = new JMenu();
  JMenuItem jMenuFileExit = new JMenuItem();
  JMenu jMenuOperate = new JMenu();
  JMenuItem jMenuOperateUnRev = new JMenuItem(); //undel
  JMenuItem jMenuLoadAllRTDB = new JMenuItem(); //加载全实时库
  JMenu jMenuHelp = new JMenu();
  JMenuItem jMenuHelpAbout = new JMenuItem();

  JToolBar jToolBar = new JToolBar();
  JButton jButtonLogin = new JButton();
  JButton jButtonDelete = new JButton();
  JButton jButtonAddRecord = new JButton();
  JButton jButtonSave = new JButton();
  JButton jButtonLoadRDB = new JButton();
  JButton jButtonInsertRecord = new JButton();
  //-----hx
  JButton jButtonhelp = new JButton();
  JButton jButtonoper = new JButton();
  JButton jButtonExecl = new JButton();

  //hx
  JButton jButtonVerify = new JButton(); //检查数据库相关表的一些字段是否存在重复
    //hx
    JButton jButtonCopy = new JButton(); //间隔复制 add by hongxiang
    JButton jButtonDevCopy = new JButton(); //设备复制 add by hongxiang

  public JLabel  jnodeinfo =new JLabel();
  JToolBar toolbar = new JToolBar();

  JSeparator sep = new JSeparator(SwingConstants.VERTICAL);
  JSeparator sep1 = new JSeparator(SwingConstants.VERTICAL);
  JSeparator sep2 = new JSeparator(SwingConstants.VERTICAL);
  JSeparator sep3 = new JSeparator(SwingConstants.VERTICAL);


  //---
  JButton jButtonFind =new JButton();
  JPanel  jPanleleft = new JPanel();
  JTextField FindTxt = new JTextField();
  JToolBar jToolBarFind = new JToolBar();
  public int findid=0;
  public boolean findflag =true;
  public DefaultMutableTreeNode flnode;
  //----hx
  ImageIcon imagelogin, imagedel, imagesave, imageloadrdb, imageadd,
  imageinsert,imagefind,imageoper,imageexport,imageverify,imagehelp;
  ImageIcon imageMenuFileExit, imageMenuUnDel, imageMenuLoadRDB,
  imageMenuLinkDB, imageMenuBcp, imagetool,imagetitle;
  ImageIcon imagepopclose, imagepopcloseall,imagecopy,imagepaste,imagerefresh,imagemhgl;

    ImageIcon imageCopy;    //间隔复制 add by hongxiang 2015/7/10
    ImageIcon imageDevCopy;    //设备复制 add by hongxiang 2016/7/20

  BorderLayout borderLayout3 = new BorderLayout();

  JPanel jstatuspanel = new JPanel();
  JPanel userpanel = new JPanel();
  JPanel statuspanel = new JPanel();

  public JLabel userstatusbar = new JLabel();
  JLabel statusBar = new JLabel();
  public JLabel infoBar = new JLabel();

  BorderLayout borderLayout1 = new BorderLayout();

  JSplitPane jSplitPane1 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);

  JScrollPane jScrollPane1Tree = new JScrollPane();

  JTabbedPane jTabbedPane1 = new JTabbedPane();

  GridLayout gridLayout1 = new GridLayout();
  BorderLayout borderLayout2 = new BorderLayout();

  Dlllink pdll = Dlllink.getInstance();

  ListTree jTree1 = new ListTree(jTabbedPane1);
  DefaultMutableTreeNode f_node;//hx  jtree-find
  JMenuItem jMenuItemLinkDB = new JMenuItem();
  JMenuItem jMenuItemMakeBcp = new JMenuItem();
  Timer timer;
  static CopyPaste copyPaste = new CopyPaste();
  private JMenuItem jMenuCheckCode = new JMenuItem();
    private String str;

    //Construct the frame
  public FrameDB() {
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    try {
      jbInit();
    }
    catch (Exception e) {
      e.printStackTrace();
   //   System.out.println("程序启动1");
    }
  }

  //主界面初始化
  private void jbInit() throws Exception {
      try {
          //菜单栏前半部分
          imagelogin = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/login_out.png"));
          imageadd = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/add_record.png"));
          imagedel = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/del_record.png"));
          imageinsert = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/insert_record.png"));
          imagesave = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/save_record.png"));
          imageloadrdb = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/load_record.png"));
          imageCopy=new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/sub_copy.png"));
          imageDevCopy=new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/dev_copy.png"));

          //菜单栏后半部分
          imageexport = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/export_excl.png"));
          imageoper = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/oper_record.png"));
          imageverify = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/verify_db.png"));

          //左侧栏相关
          imagefind = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/find_node.png"));
          imagerefresh = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/refresh_cell.png"));
          imagecopy = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/copy_cell.png"));
          imagepaste = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/paste_cell.png"));
          imagemhgl= new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/filter_cell.png"));
//          imagerefresh = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/refresh.png"));
//          imagecopy = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/copy.png"));
//          imagepaste = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/paste.png"));
//          imagemhgl= new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/mhgl.png"));

          //关闭表相关
          imagepopclose = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/close_a_table.png"));
          imagepopcloseall = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/close_all_table.png"));

          //其他
          imagehelp = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/help.png"));
          imageMenuFileExit = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/findnode.png"));
          imageMenuUnDel = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/findnode.png"));
          imageMenuLoadRDB = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/findnode.png"));
          imageMenuLinkDB = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/findnode.png"));
          imageMenuBcp = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/findnode.png"));
          imagetool = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/findnode.png"));
          imagetitle=new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/dbconfig.png"));
      }
    catch (NullPointerException npe) {}
   // System.out.println("程序启动2");
    //setIconImage(Toolkit.getDefaultToolkit().createImage(FrameDB.class.getResource("[Your Icon]")));
    contentPane = (JPanel)this.getContentPane();
    this.setDefaultCloseOperation(3);
    this.setForeground(UIManager.getColor("control"));
    this.setIconImage(imagetitle.getImage());
    this.setTitle("D6000模型维护工具");
      // 设置最大化显示
      this.setExtendedState(Frame.MAXIMIZED_BOTH);
    this.addWindowListener(new java.awt.event.WindowAdapter() {
      public void windowClosing(WindowEvent e) {
        this_windowClosing(e);
      }
    });
    statusBar.setText(" ");
    jMenuFile.setText("文件");
    jMenuFileExit.setText("退出");
    jMenuOperateUnRev.setText("UnDel");
    jMenuLoadAllRTDB.setText("加载全实时库");

    jMenuFileExit.addActionListener(new jMenuFileExitActionListener());

    jMenuOperateUnRev.addActionListener(new jMenuOperateUnRevActionListener());

    jMenuLoadAllRTDB.addActionListener(new jMenuLoadAllRTDBActionLIstener());

    jMenuHelp.setText("帮助");
    jMenuOperate.setText("工具");
    jMenuHelpAbout.setText("About");
    jMenuHelpAbout.addActionListener(new jMenuHelpAboutActionListener());
    jButtonLogin.addActionListener(new jButtonLoginActionListener());

    jButtonDelete.addActionListener(new jButtonDeleteActionListener());
    jButtonAddRecord.addActionListener(new jButtonAddRecordActionListener());
    jButtonInsertRecord.addActionListener(new jButtonInsertRecordActionListener());
    //jButtonFind
    jButtonFind.addActionListener(new jButtonFindActionListener());
    jButtonoper.addActionListener(new jButtonOperActionListener());
    jButtonExecl.addActionListener(new jButtonExeclctionListener());
    jButtonhelp.addActionListener(new jButtonHelpActionListener());
    jButtonSave.addActionListener(new jButtonSavaActionListener());
    jButtonLoadRDB.addActionListener(new jButtonLoadRDBActionListener());
    jTree1.addTreeSelectionListener(new jTreeSelecttionListener());
    jTree1.addMouseMotionListener(new jTreeMouseMotionListener()); //用于判断是否在框架上移动了鼠标
    jTabbedPane1.addChangeListener(new jTabbedPanelChangeListener());
    jTabbedPane1.addMouseMotionListener(new jTabbedPanelMouseMotionListener());

      jButtonVerify.addActionListener(new jButtonVerifyActionListener());
      jButtonCopy.addActionListener(new jButtonCopyActionListener());
      jButtonDevCopy.addActionListener(new jButtonDevCopyActionListener());

      jButtonLogin.setIcon(imagelogin);
    jButtonLogin.setToolTipText("用户登录");
    jButtonLogin.setText("登录");
    contentPane.setLayout(borderLayout1);
    //----- hx
    jButtonoper.setToolTipText("操作");
    jButtonoper.setText("操作记录");
    jButtonoper.setIcon(imageoper);
    jButtonhelp.setToolTipText("帮助");
    jButtonhelp.setText("帮助");
    jButtonhelp.setIcon(imagehelp);

    jButtonExecl.setToolTipText("导出");
    jButtonExecl.setText("导出");
    jButtonExecl.setIcon(imageexport);

      jButtonVerify.setToolTipText("模型校验");
      jButtonVerify.setText("模型校验");
      jButtonVerify.setIcon(imageverify);

      jButtonCopy.setToolTipText("站所复制");
      jButtonCopy.setText("站所复制");
      jButtonCopy.setIcon(imageCopy);

      jButtonDevCopy.setToolTipText("设备复制");
      jButtonDevCopy.setText("设备复制");
      jButtonDevCopy.setIcon(imageDevCopy);


    //-----

    jButtonDelete.setToolTipText("删除");
    jButtonDelete.setText("删除");
    jButtonDelete.setIcon(imagedel);
    jButtonAddRecord.setIcon(imageadd);
    jButtonInsertRecord.setIcon(imageinsert);

    jButtonSave.setToolTipText("存盘");
    jButtonSave.setText("存盘");
    jButtonSave.setIcon(imagesave);
    jButtonLoadRDB.setIcon(imageloadrdb);

    //begin set background
    contentPane.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                        ConstantDef.CBLUE));
    setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                            ConstantDef.CBLUE));
    jMenuBar1.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                      ConstantDef.CBLUE));
    jMenuFile.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                      ConstantDef.CBLUE));
    jMenuFileExit.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                          ConstantDef.CBLUE));
    jMenuOperate.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                         ConstantDef.CBLUE));
    jMenuOperateUnRev.setBackground(new Color(ConstantDef.CRED,
                                              ConstantDef.CGREEN,
                                              ConstantDef.CBLUE));
    jMenuLoadAllRTDB.setBackground(new Color(ConstantDef.CRED,
                                             ConstantDef.CGREEN,
                                             ConstantDef.CBLUE));
    jMenuHelp.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                      ConstantDef.CBLUE));
    jMenuHelpAbout.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                           ConstantDef.CBLUE));
    jMenuItemLinkDB.setBackground(new Color(ConstantDef.CRED,
                                            ConstantDef.CGREEN,
                                            ConstantDef.CBLUE));
    jMenuItemMakeBcp.setBackground(new Color(ConstantDef.CRED,
                                             ConstantDef.CGREEN,
                                             ConstantDef.CBLUE));
    jMenuCheckCode.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                           ConstantDef.CBLUE));
    jTabbedPane1.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                         ConstantDef.CBLUE));

    jstatuspanel.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                         ConstantDef.CBLUE));
    userpanel.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                      ConstantDef.CBLUE));
    statuspanel.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                        ConstantDef.CBLUE));
    jSplitPane1.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                        ConstantDef.CBLUE));
   // jScrollPane.getViewport().setBackground(Color.white);

    contentPane.setMinimumSize(new Dimension(180, 105));

    jTabbedPane1.addMouseListener(new java.awt.event.MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        jTabbedPane1_mouseClicked(e);
      }
    });

    //添加弹出式菜单
    jTree1.addMouseListener(new java.awt.event.MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        jTree1_mouseClicked(e);
      }
    });

    //处理ESCAPE、DELETE键的输入
    jTabbedPane1.addKeyListener(new java.awt.event.KeyAdapter() {
      public void keyPressed(KeyEvent e) {
        jTree1.doPressKey(e);
      } //public void keyTyped(KeyEvent e) {
    });

    jTabbedPane1.setMinimumSize(new Dimension(80, 50));
    jTabbedPane1.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jTabbedPane1.setPreferredSize(new Dimension(800, 500));
    jTree1.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, 14));
    jTree1.setRowHeight(20);
    jTree1.setToggleClickCount(1); //单击展开
    jTree1.setScrollsOnExpand(true);

    jScrollPane1Tree.setToolTipText("");
    jMenuItemLinkDB.setText("连接数据库");

    jMenuItemLinkDB.setEnabled(false); //数据库连接不上时，ConstantDef.TAB1ID不能正确赋值

    jMenuItemLinkDB.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jMenuItemLinkDB_actionPerformed(e);
      }
    });

    jMenuItemMakeBcp.setText("生成bcp");
    jMenuItemMakeBcp.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jMenuItemMakeBcp_actionPerformed(e);
      }
    });

    timer = new Timer(20000, new TimerActionListener());

    timer.start();
    infoBar.setBorder(BorderFactory.createLoweredBevelBorder());
    userpanel.setBorder(BorderFactory.createLoweredBevelBorder());
    jMenuCheckCode.setText("代码检查");
    jMenuCheckCode.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jMenuCheckCode_actionPerformed(e);
      }
    });
    jButtonAddRecord.setActionCommand("增加");
    jButtonInsertRecord.setActionCommand("插入");
    jButtonLoadRDB.setActionCommand("加载");
    jButtonAddRecord.setToolTipText("增加");
    jButtonAddRecord.setText("增加");
    jButtonInsertRecord.setToolTipText("插入");
    jButtonInsertRecord.setText("插入");
    jButtonLoadRDB.setToolTipText("加载");
    jButtonLoadRDB.setText("加载");
    jButtonLoadRDB.setBorder(BorderFactory.createEtchedBorder());
    jButtonDelete.setBorder(BorderFactory.createEtchedBorder());
    jButtonInsertRecord.setBorder(BorderFactory.createEtchedBorder());
    jButtonSave.setBorder(BorderFactory.createEtchedBorder());
    jButtonLogin.setBorder(BorderFactory.createEtchedBorder());
    jButtonAddRecord.setBorder(BorderFactory.createEtchedBorder());
    //---
    jButtonoper.setBorder(BorderFactory.createEtchedBorder());
    jButtonhelp.setBorder(BorderFactory.createEtchedBorder());
    jButtonExecl.setBorder(BorderFactory.createEtchedBorder());
      jButtonVerify.setBorder(BorderFactory.createEtchedBorder());
      jButtonCopy.setBorder(BorderFactory.createEtchedBorder());
      jButtonDevCopy.setBorder(BorderFactory.createEtchedBorder());
    //---
    jToolBar.setBackground(new Color(238, 238, 238));
    jButtonLoadRDB.setBackground(new Color(238, 238, 238));
    jButtonSave.setBackground(new Color(238, 238, 238));
    jButtonInsertRecord.setBackground(new Color(238, 238, 238));
    jButtonAddRecord.setBackground(new Color(238, 238, 238));
    jButtonDelete.setBackground(new Color(238, 238, 238));
    jButtonLogin.setBackground(new Color(238, 238, 238));
    jButtonoper.setBackground(new Color(238, 238, 238));
    jButtonhelp.setBackground(new Color(238, 238, 238));
    jButtonExecl.setBackground(new Color(238, 238, 238));
      jButtonVerify.setBackground(new Color(238, 238, 238));

    jButtonDelete.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonLogin.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonAddRecord.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonInsertRecord.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonSave.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonLoadRDB.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonoper.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonhelp.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
    jButtonExecl.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
      jButtonVerify.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));
      jButtonDevCopy.setFont(new java.awt.Font(ConstantDef.FONTNAME, 1, 12));

    sep.setPreferredSize(new Dimension(10, 10));
    sep.setMaximumSize(new Dimension(10, 10));
    sep.setMinimumSize(new Dimension(10, 10));

      sep1.setPreferredSize(new Dimension(10,10 ));
    sep1.setMaximumSize(new Dimension(10, 10));
    sep1.setMinimumSize(new Dimension(10, 10));

      sep2.setPreferredSize(new Dimension(10,10 ));
    sep2.setMaximumSize(new Dimension(10, 10));
    sep2.setMinimumSize(new Dimension(10, 10));

      sep3.setPreferredSize(new Dimension(10,10 ));
    sep3.setMaximumSize(new Dimension(10, 10));
    sep3.setMinimumSize(new Dimension(10, 10));

      //菜单栏
      jToolBar.add(jButtonLogin);
      jToolBar.add(sep1);

      jToolBar.add(jButtonAddRecord); //增加记录
      jToolBar.add(jButtonDelete);
      jToolBar.add(jButtonInsertRecord); //插入记录
      jToolBar.add(sep2);

      jToolBar.add(jButtonSave); //存盘
      jToolBar.add(jButtonLoadRDB);
      jToolBar.add(sep3);
      jToolBar.add(jButtonDevCopy);
      jToolBar.add(jButtonCopy);
      jToolBar.add(jButtonVerify);

      jToolBar.add(sep);
      jToolBar.add(jButtonExecl);
      jToolBar.add(jButtonoper);
//    jToolBar.add(jButtonhelp);
      jToolBar.add(jnodeinfo);

    jnodeinfo.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, 14));
    jnodeinfo.setForeground(Color.blue);
    jnodeinfo.setBackground(new Color(238,238,238));
    jMenuFile.add(jMenuFileExit);
    jMenuFileExit.setIcon(imageMenuFileExit);
    jMenuOperate.add(jMenuOperateUnRev);
    jMenuOperateUnRev.setIcon(imageMenuUnDel);

    jMenuOperate.add(jMenuLoadAllRTDB);
    jMenuLoadAllRTDB.setIcon(imageMenuLoadRDB);
    jMenuOperate.add(jMenuItemLinkDB);
    jMenuItemLinkDB.setIcon(imageMenuLinkDB);
    jMenuOperate.add(jMenuItemMakeBcp);
    jMenuOperate.add(jMenuCheckCode);
    jMenuItemMakeBcp.setIcon(imageMenuBcp);

    jMenuHelp.add(jMenuHelpAbout);
    jMenuBar1.add(jMenuFile);
    jMenuBar1.add(jMenuOperate);
    jMenuOperate.setIcon(imagetool);

    jMenuBar1.setFont(new Font(ConstantDef.FONTNAME, 0, 12));

    jMenuBar1.add(jMenuHelp);
  //  this.setJMenuBar(jMenuBar1);
    contentPane.add(jToolBar, BorderLayout.NORTH);

    GridLayout gridlayout = new GridLayout();
    gridlayout.setColumns(3);
    gridlayout.setRows(1);

    jstatuspanel.setLayout(gridlayout);
    jstatuspanel.setPreferredSize(new Dimension(24, 24));
    contentPane.add(jstatuspanel, BorderLayout.SOUTH);
    contentPane.add(jSplitPane1, BorderLayout.CENTER);

    jstatuspanel.add(statuspanel);
    jstatuspanel.add(infoBar);
    jstatuspanel.add(userpanel);

    statuspanel.setBorder(BorderFactory.createLoweredBevelBorder());
    statuspanel.add(statusBar, BorderLayout.CENTER);
    userpanel.add(userstatusbar, BorderLayout.CENTER);
    infoBar.setForeground(Color.blue);
    GridLayout gridlayout1 = new GridLayout();
    gridlayout.setColumns(1);
    gridlayout.setRows(1);

    infoBar.setLayout(gridlayout);

  //  jSplitPane1.add(jScrollPane1Tree, JSplitPane.LEFT);
  //  jScrollPane1Tree.setPreferredSize(new Dimension(220, 0));
  jSplitPane1.add(jPanleleft, JSplitPane.LEFT);
  jPanleleft.setPreferredSize(new Dimension(220, 0));

  jSplitPane1.add(jTabbedPane1, JSplitPane.RIGHT);
  jSplitPane1.setDividerSize(3);

  jPanleleft.setLayout(new BorderLayout());
  jPanleleft.add(jScrollPane1Tree, BorderLayout.CENTER);
    jPanleleft.add(jToolBarFind,BorderLayout.NORTH);
    jToolBarFind.setPreferredSize(new Dimension(20, 30));
    jScrollPane1Tree.getViewport().add(jTree1, null);
    jToolBarFind.setFloatable(false);
    jButtonFind.setBackground(new Color(238,238,238));
    jButtonFind.setIcon(imagefind);
    jToolBarFind.add(FindTxt);
    jToolBarFind.add(jButtonFind);

    ToolTipManager.sharedInstance().registerComponent(jTree1);

    try {
      Dlllink.dbstatus = pdll.startdb();
    }
    catch (UnsatisfiedLinkError ule) {
      // ule.printStackTrace();
      JOptionPane.showMessageDialog(null, "动态库dblink连接失败!", "发送信息提示",
                                    JOptionPane.ERROR_MESSAGE);
      System.exit(0);
    }

    if (Dlllink.dbstatus == 0) {
      ConstantDef.statusText = "数据库连接异常";
    }
    else {
      ConstantDef.statusText = "数据库连接正常";
    }

    statusBar.setText(ConstantDef.statusText);

    Dlllink.runhome = new String(pdll.getenvvar()); //得到环境变量
    jTree1.pdb_fieldtable = new PDB_FieldTable();
    PDB_Table pdbtable = new PDB_Table();
    byte[][] logtimelong = pdll.querydbinfoini("项目名='注销登录时限'");
    if (logtimelong.length > 0) {
      ConstantDef.logoutTime = Integer.parseInt(pdll.substr(logtimelong[0]));
    }
    setButton(false);
    jToolBar.setFloatable(false);
    jSplitPane1.setDividerLocation(200);
  }

  //Overridden so we can exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      jMenuFileExit_actionPerformed(null);
    }
  }

  public void jTabbedPane1_mouseClicked(MouseEvent e) {
    if (e.getID() == e.MOUSE_CLICKED) {
      if ( (e.getModifiers() & e.BUTTON3_MASK) == e.BUTTON3_MASK) {
        JPopupMenu popmenu = new JPopupMenu("Table Operater");
        JMenuItem jMenuCloseTable = new JMenuItem();
        JMenuItem jMenuCloseAllTable = new JMenuItem();
        jMenuCloseTable.setText("关闭当前表");
        jMenuCloseTable.setIcon(imagepopclose);
        jMenuCloseAllTable.setText("关闭所有表");
        jMenuCloseAllTable.setIcon(imagepopcloseall);
        popmenu.add(jMenuCloseTable);
        popmenu.add(jMenuCloseAllTable);
        if (jTabbedPane1.getTabCount() > 0) {
          popmenu.show(e.getComponent(), e.getX(), e.getY());

        }
        jMenuCloseTable.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
            int tabindex = jTabbedPane1.getSelectedIndex();
            if (tabindex < 0) {
              return;
            }
            boolean rtn = jTree1.removeATab(tabindex, true);
            int tabindex1 = jTabbedPane1.getSelectedIndex();
            if (tabindex1 >= 0)
              infoBar.setText(jTree1.vTitle.get(tabindex1).toString());
            else
              infoBar.setText("");
            if (jTabbedPane1.getTabCount() < 1) {
              setButton(false);
            }
          }
        });
        jMenuCloseAllTable.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
            int tabcount = jTabbedPane1.getTabCount();
            for (int i = 0; i < tabcount; i++) {
              boolean rtn = jTree1.removeATab(tabcount - i - 1, true);
            }
            setButton(false);
            infoBar.setText("");
          }
        });
        setRecordNums();
      } //if
    } //if
  } //void

  void jTree1_mouseClicked(MouseEvent e) {
    if (e.getID() == e.MOUSE_CLICKED) {
      if ( (e.getModifiers() & e.BUTTON3_MASK) == e.BUTTON3_MASK) {

        JPopupMenu popmenu = new JPopupMenu("refresh");
        JMenuItem jMenuFresh = new JMenuItem();
        jMenuFresh.setText("刷新");
        jMenuFresh.setIcon(imagerefresh);
        popmenu.add(jMenuFresh);

        JMenuItem jMenuCopy = new JMenuItem();
        jMenuCopy.setText("复制");
        jMenuCopy.setIcon(imagecopy);
        popmenu.add(jMenuCopy);

        JMenuItem jMenuPaste = new JMenuItem();
        jMenuPaste.setText("粘贴");
        jMenuPaste.setIcon(imagepaste);
        popmenu.add(jMenuPaste);

        JMenuItem jMenumhgl = new JMenuItem();
        jMenumhgl.setText("过滤");
        jMenumhgl.setIcon(imagemhgl);
        popmenu.add(jMenumhgl);



        final DefaultMutableTreeNode node = (DefaultMutableTreeNode) jTree1.
            getLastSelectedPathComponent();

        jMenuPaste.setEnabled(false);
        jMenuCopy.setEnabled(false);
        jMenumhgl.setEnabled(false);
        if(node !=null){
          if (node.isLeaf() || node.getLevel() == 2 ) {
        //  if (node.getLevel()>1 && node.getLevel()<5) {
            jMenumhgl.setEnabled(true);
          }
        }
        else
          return;
        if (node.getLevel() > 2) {
          if (node.isLeaf()) {
            if (ConstantDef.getUserInstance().getEditRight() != 0) {
              jMenuPaste.setEnabled(true);
            }
            jMenuCopy.setEnabled(true);

          }
        }
        //CIM模型专用  添加区域和厂站的编辑
        if(node.getLevel()==1 && node.toString().equalsIgnoreCase("CIM设备模型")){
          JMenuItem jMenuArea = new JMenuItem();
          jMenuArea.setText("编辑区域");
          popmenu.add(jMenuArea);
          JMenuItem jMenuStation = new JMenuItem();
          jMenuStation.setText("编辑厂站");
          popmenu.add(jMenuStation);
          jMenuArea.addActionListener(new ActionListener() {
           public void actionPerformed(ActionEvent e1) {
             jTree1.OpenArea(node,node.toString(),node.getLevel());
           }
         });
          jMenuStation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenStation(node, node.toString(), node.getLevel());
            }
          });
        }
        //CIM模型专用  添加区域和厂站的编辑
        if (node.getLevel() == 2 && node.getParent().toString().equalsIgnoreCase("CIM设备模型")) {

          JMenuItem jMenuStation = new JMenuItem();
          jMenuStation.setText("编辑厂站");
          popmenu.add(jMenuStation);
          jMenuStation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenAStation(node, node.toString(), node.getLevel());
            }
          });
        }

        //CIM模型专用  添加电压等级和设备的编辑
        if (node.getLevel() == 3 && node.getParent().getParent().toString().equalsIgnoreCase("CIM设备模型")) {
          JMenuItem jMenuArea = new JMenuItem();
          jMenuArea.setText("编辑电压等级");
          popmenu.add(jMenuArea);
          JMenuItem jMenuStation = new JMenuItem();
          jMenuStation.setText("编辑SCD设备");
          popmenu.add(jMenuStation);
          jMenuArea.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenVoltage(node, node.toString(), node.getLevel());
            }
          });
          jMenuStation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenSDev(node, node.toString(), node.getLevel());
            }
          });
        }
        //CIM模型专用  添加电压等级和设备的编辑
        if (node.getLevel() == 4 && node.getParent().getParent().getParent().toString().equalsIgnoreCase("CIM设备模型")) {

          /*
           JMenuItem jMenuArea = new JMenuItem();
          jMenuArea.setText("编辑SCD类型");
          popmenu.add(jMenuArea);
          JMenuItem jMenuStation = new JMenuItem();
          jMenuStation.setText("编辑SCD设备");
          popmenu.add(jMenuStation);
          jMenuArea.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenDevType(node, node.toString(), node.getLevel());
            }
          });
          jMenuStation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenSVDev(node, node.toString(), node.getLevel());
            }
          });
          */
         JMenuItem jMenuArea = new JMenuItem();
        jMenuArea.setText("编辑间隔参数");
        popmenu.add(jMenuArea);
        JMenuItem jMenuStation = new JMenuItem();
        jMenuStation.setText("编辑SCD设备");
        popmenu.add(jMenuStation);
        jMenuArea.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e1) {
            jTree1.OpenSeparate(node, node.toString(), node.getLevel());
          }
        });
        jMenuStation.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e1) {
            jTree1.OpenSVDev(node, node.toString(), node.getLevel());
          }
        });

        }
        //CIM模型专用  设备的编辑
        if (node.getLevel() == 5 &&
            node.getParent().getParent().getParent().getParent().toString().
            equalsIgnoreCase("CIM设备模型")) {
          JMenuItem jMenuArea = new JMenuItem();
          jMenuArea.setText("编辑SCD设备");
          popmenu.add(jMenuArea);

          jMenuArea.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e1) {
              jTree1.OpenSeparateDev(node, node.toString(), node.getLevel());
            }
          });
        }
        //end
        popmenu.show(e.getComponent(), e.getX(), e.getY());

        int xpos = e.getX();
        int ypos = e.getY();
        TreePath selPath = jTree1.getPathForLocation(xpos, ypos);

        jMenuFresh.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e1) {
            if (ConstantDef.getUserInstance().getBrowserPara() == 0) {
              return;
            }
            int level = node.getLevel();
            if (node.isLeaf()) { //叶子节点的刷新是:判断保存，保存错误，将不刷新
              boolean bExpandNodes = true;
              bExpandNodes = removeTreeLeafNode(node, true);
              if (bExpandNodes) {
                jTree1.addBranchNodes(node, node.toString(), level);
              }
              setRecordNums();
            } //if(node.isLeaf() )
            else { //非叶子节点,找出所有的叶子节点，判断是否要删除，叶子节点在JTab中全部删除成功后，在重构此节点
              boolean bExpandNodes = removeTreeNode(node, true);
              if (bExpandNodes) {
                node.removeAllChildren();
                jTree1.updateUI();
                jTree1.addBranchNodes(node, node.toString(), level);
              }
            }
          }
        });

        jMenuCopy.addActionListener(new ActionListener() { //拷贝
          public void actionPerformed(ActionEvent e1) {
            if (isValidNode(node)) {
              int tabindex = jTabbedPane1.getSelectedIndex();
              copyPaste.setNode(node);
              int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.
                                                elementAt(
                  tabindex).toString()); //分区数
              copyPaste.dataVector = new Vector();
              MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(
                  tabindex);
              MyTableModel myTableModel1 = (MyTableModel) myjtable.getModel();

              if (PreProcessor.TableLock) { //表互斥修改  ---洪祥，判断表是否在修改
                byte[][] buf;
                int[] fieldcount = new int[1];
                String wh;
                wh = "tablename = '" + myTableModel1.getTableName() + "'";
                buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                                         "tablename", wh, "", fieldcount, false);
                int len = buf.length;
                if (myTableModel1.getSaveFlag() == false) {
                  if (len > 0) {
                    JOptionPane.showMessageDialog(null, "此表正在编辑中", "提示",
                                                  JOptionPane.WARNING_MESSAGE);
                    return;
                  }
                }
                //表互斥修改
              }
              for (int i = 0; i < myTableModel1.dataVector.size(); i++) {
                copyPaste.dataVector.addElement( ( (Vector) (myTableModel1.
                    dataVector.elementAt(i))).clone());
                if (tabnocount > 1) {
                  for (int j = 0; j < tabnocount - 1; j++) {
                    MyJTable jtabtmp = myjtable.getJTable(j);
                    MyTableModel myTabnoTableModel2 = (MyTableModel) jtabtmp.
                        getModel();
                    copyPaste.dataVector.addElement( ( (Vector) (
                        myTabnoTableModel2.
                        dataVector.elementAt(i))).clone());
                  }
                }
              }
            }
            else {
              JOptionPane.showMessageDialog(null, "表不存在，无法复制!", "操作提示信息",
                                            JOptionPane.INFORMATION_MESSAGE);
              return;
            }
          }
        });

        jMenuPaste.addActionListener(new ActionListener() { //paste
          public void actionPerformed(ActionEvent e1) {
            if (isValidNode(node)) {
              int level = node.getLevel();
              if (level == 3 &&
                  !node.getParent().equals(copyPaste.node.getParent())) {
                JOptionPane.showMessageDialog(null, "表不对，无法粘贴!", "操作提示信息",
                                              JOptionPane.INFORMATION_MESSAGE);
                return;
              }
              else if (level == 4 &&
                       !node.getParent().getParent().equals(copyPaste.node.
                  getParent().getParent())) {
                JOptionPane.showMessageDialog(null, "表不对，无法粘贴!", "操作提示信息",
                                              JOptionPane.INFORMATION_MESSAGE);
                return;
              }
              int tabindex = jTabbedPane1.getSelectedIndex();
              int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.
                                                elementAt(
                  tabindex).toString()); //分区数

              MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(
                  tabindex);
              MyTableModel myTableModel1 = (MyTableModel) myjtable.getModel();
          if (PreProcessor.TableLock) {
            //表互斥修改  ---洪祥，判断表是否在修改
            byte[][] buf;
            int[] fieldcount = new int[1];
            String wh;
            wh = "tablename = '" + myTableModel1.getTableName() + "'";
            buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                                     "tablename", wh, "", fieldcount, false);
            int len = buf.length;
            if (myTableModel1.getSaveFlag() == false) {
              if (len > 0) {
                JOptionPane.showMessageDialog(null, "此表正在编辑中", "提示",
                                              JOptionPane.WARNING_MESSAGE);
                return;
              }
            }
            //表互斥修改
          }
              for (int i = 0; i < copyPaste.dataVector.size() / tabnocount; i++) {
                myTableModel1.addRow( (Vector) (copyPaste.dataVector.
                                                elementAt(i * tabnocount)));
                myTableModel1.managemodel.addArec();
                if (myTableModel1.getListFieldFlag()) {
                  myTableModel1.addListField("");
                }

                if (tabnocount > 1) {
                  for (int j = 0; j < tabnocount - 1; j++) {
                    MyJTable jtabtmp = myjtable.getJTable(j);
                    MyTableModel mytabletmp = (MyTableModel) jtabtmp.getModel();
                    Vector dv = (Vector) (copyPaste.dataVector.elementAt(i *
                        tabnocount + j + 1));
                    mytabletmp.addListField(mytabletmp.getRowCount()); //jhongtu ++ 20050419
                    mytabletmp.addRow(dv);
                  }
                }
              }
              if (copyPaste.dataVector.size() > 0) {
                jTabbedPane1.setForegroundAt(tabindex, Color.red);
                setRecordNums();
              }
            }
            else {
              JOptionPane.showMessageDialog(null, "表不存在，无法粘贴!", "操作提示信息",
                                            JOptionPane.INFORMATION_MESSAGE);
              return;
            }
          }
        });
        //筛选界面
        jMenumhgl.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e1) {
                flnode=node;
                FilterDialog filterdlg = new FilterDialog(
                    ApplicationDB.frameDB, node.toString().trim(), true);

                filterdlg.setModal(true);
                filterdlg.setLocationRelativeTo(null);
                filterdlg.show();
                filterdlg.pack();
                jTree1.flwhere="";
          //  } //if(node.isLeaf() )

          }
        });

      }
    }
  }

//是否有效node,防止表不存在的情况
  boolean isValidNode(DefaultMutableTreeNode node) {
    int tabindex = jTabbedPane1.getSelectedIndex();
    if (tabindex < 0) {
      return false;
    }
    else {
      String sTabTitle = jTabbedPane1.getTitleAt(tabindex);
      String title1 = "";
      title1 = node.toString().trim();
      int level = node.getLevel();
      DefaultMutableTreeNode node1 = node;
      for (int i = 2; i < level; i++) {
        node1 = (DefaultMutableTreeNode) node1.getParent();
        title1 = node1.toString().trim() + "/" + title1;
      }
      if (!sTabTitle.equals(title1)) {
        return false;
      }
    } //if(tabindex >= 0)
    return true;
  }

  //删除非叶子节点的所有子节点，在JTabbedPane中的显示
  boolean removeTreeNode(DefaultMutableTreeNode node, boolean bSaveFlag) {
    boolean bResult = true;
    if (node.isLeaf()) {
      return removeTreeLeafNode(node, bSaveFlag);
    }
    int childcount = node.getChildCount();
    for (int i = 0; i < childcount; i++) {
      DefaultMutableTreeNode node1 = (DefaultMutableTreeNode) node.getChildAt(i); //开始删除节点
      if (!removeTreeNode(node1, bSaveFlag)) {
        return false;
      }
    }
    return bResult;
  }

  public  boolean removeTreeLeafNode(DefaultMutableTreeNode node, boolean bSaveFlag) {
    int jtabcount = jTabbedPane1.getTabCount();
    boolean bResult = true;
    String nodestr = "";
    nodestr = node.toString();
    int level = node.getLevel();
    if (jtabcount > 0) {
      DefaultMutableTreeNode node1 = node;
      for (int i = 2; i < level; i++) {
        node1 = (DefaultMutableTreeNode) node1.getParent();
        nodestr = node1.toString() + "/" + nodestr;
      } //for i
      for (int i = 0; i < jtabcount; i++) {
        if (nodestr.equalsIgnoreCase(jTabbedPane1.getTitleAt(i))) {
          bResult = jTree1.removeATab(i, bSaveFlag);
          break;
        }
      } //for i
    } //if(jtabcount>0)
    return bResult;
  }

  //设置按钮的状态，根据用户的权限和表是否可编辑(视图和没有主键的表不允许编辑)
  void setButton(boolean statusf) {
    jButtonDelete.setEnabled(statusf);
    jButtonAddRecord.setEnabled(statusf);
    jButtonSave.setEnabled(statusf);
    jButtonInsertRecord.setEnabled(statusf);
    jButtonLoadRDB.setEnabled(statusf); //加载实时库按钮
//      jButtonCopy.setEnabled(statusf); //间隔复制按钮
//      jButtonVerify.setEnabled(statusf); //数据库校验按钮
  }

  public void expandNodes(DefaultMutableTreeNode node) {
    if (node == null) {
      return;
    }
    Object nodeInfo = node.getUserObject();
    int level = node.getLevel();
    String nodestr = nodeInfo.toString().trim();

    if(PreProcessor.ALIAS){
      //判断以下节点名是否是别名，如果是，则能够返回实际表名
      String tablename = GetTableNameFromAlias(nodeInfo.toString());
      if (tablename != "")
        nodestr = tablename;
    }
    String title1 = "";
    title1 = title1 + node.toString().trim();
    DefaultMutableTreeNode node1 = node;
    for (int i = 0; i < level; i++) {
      node1 = (DefaultMutableTreeNode) node1.getParent();
      title1 = node1.toString().trim() + "/" +title1 ;
    }
    title1="     树节点路径:/"+ title1;
    infoBar.setText(title1);
    if (node.isLeaf()) {
      jTree1.addBranchNodes(node, nodestr, level);
    }
    return;
  }

  void this_windowClosing(WindowEvent e) {
    int tabcount = jTabbedPane1.getTabCount();
    for (int i = 0; i < tabcount; i++) {
      boolean rtn = jTree1.removeATab(tabcount - i - 1, true);
}

    pdll.closedb();
    System.exit(0);
  }

  void jMenuItemLinkDB_actionPerformed(ActionEvent e) {
    Dlllink.dbstatus = pdll.startdb();
    if (Dlllink.dbstatus == 0) {
      statusBar.setText("数据库连接失败");
    }
    else {
      statusBar.setText("数据库连接成功");
    }
  }

  void jMenuItemMakeBcp_actionPerformed(ActionEvent e) {

    BcpDialog bcpdlg = new BcpDialog(this, "生成bcp文件", true);
    bcpdlg.show();
  }

  public void removeSysRootNodes() {
    int childnums = ListTree.rootnode.getChildCount();
    if (jTree1.serviceTypeTable == null) {
      return;
    }
    if (childnums < jTree1.serviceTypeTable.length) {
      return; //子节点数少，说明系统应用类型没有显示
    }
    String sysdes = "";
    int k;
    for (k = 0; k < jTree1.serviceTypeTable.length; k++) {
      if (jTree1.serviceTypeTable[k].serviceType.equalsIgnoreCase(ConstantDef.
          SYSTEM_DM)) {
        sysdes = jTree1.serviceTypeTable[k].serviceDescrible;
        break;
      }
    }
    if (sysdes.equals("")) {
      return;
    }
    for (int i = 0; i < childnums; i++) {
      DefaultMutableTreeNode treenode = (DefaultMutableTreeNode) ListTree.
          rootnode.getChildAt(childnums - i - 1);
      if (treenode.toString().equalsIgnoreCase(sysdes)) {
        removeTreeNode(treenode, false);
        ListTree.rootnode.remove(childnums - i - 1);
        jTree1.serviceTypeTable[k].isVisible = false;
        break;
      }
    }
  }

  //根据用户权限来更新节点显示
  void updateShow() {
    User user = ConstantDef.getUserInstance();
    //编辑和浏览是用户节点权限表中设置的权限
    if (user.getBrowserPara() == 0 || user.getEditPara() == 0) { //不关闭所有表
      setButton(false);
    }
    else if (user.getEditPara() > 0) { //有编辑权限
      int tabcount = jTabbedPane1.getTabCount();
      if (tabcount > 0) {
        int tabindex = jTabbedPane1.getSelectedIndex();
        if (tabindex >= 0) {
          int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.elementAt(
              tabindex).toString().trim());
          MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(tabindex);
          if (tabnocount > 1) {
            JTabbedPane jtabbedPaneTabno = (JTabbedPane) jTree1.vTabnoJTab.
                elementAt(tabindex);
            int index2 = jtabbedPaneTabno.getSelectedIndex();
            if (index2 >= 1) {
              myjtable = myjtable.getJTable(index2 - 1);
            }
          }
          if (myjtable == null) {
            return;
          }
          String tabletype = myjtable.getTableType();
          if (tabletype.equalsIgnoreCase("U")) {
            setButton(true);
          }
        } //if(tabindex >= 0)
      } // if(tabcount > 0)
      ConstantDef.setLoginTime(); //登录时的时间分
    }

    //用户口令参数表中设置的值
    if (user.isManageGroup()) { //如果是管理员组
      jTree1.addSysRootNodes();
    }
    else {
      removeSysRootNodes();
    }
    jTree1.updateUI();
  }

  //在状态条中设置记录数
  public void setRecordNums() {
    int index = jTabbedPane1.getSelectedIndex();
    if (index >= 0) {
      try {
        int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.elementAt(
            index).
                                          toString()); //分区数
        MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(index);
        MyTableModel myTableModel1 = (MyTableModel) myjtable.getModel();
        statusBar.setText(ConstantDef.statusText + "     - - -     记录个数：" + myTableModel1.getRowCount());
      }
      catch (ArrayIndexOutOfBoundsException aioe) {
        statusBar.setText(ConstantDef.statusText);
      }
      catch (Exception e) {
        statusBar.setText(ConstantDef.statusText);
      }
    } //if(index >= 0)
    else {
      statusBar.setText(ConstantDef.statusText);
    }
  }

  //由别名获得实际表名
  public String GetTableNameFromAlias(String alias){
    String table="";
    byte[][] buf;
    int[] fieldcount = new int[1];
    String wh = "alias = '" + alias.trim() + "'";
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.PARA_TABLE_DESCRIBLE,
                             "tablename", wh, "", fieldcount, false);
    if (buf.length > 0) {
      table = pdll.substr(buf[0]);
      table.trim();
    }
    return table;
  }

  public String GetAliasFromFieldName(String tableName,String fieldName){
    String field = "";
    byte[][] buf;
    int[] fieldcount = new int[1];
    String wh = "tablename='"+ tableName.trim() + "'" +
        " and DESCRIBE = '" + fieldName.trim() + "'";
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.PARA_FIELD_DESCRIBLE,
                             "alias", wh, "", fieldcount, false);
    if (buf.length > 0) {
      field = pdll.substr(buf[0]);
      field.trim();
    }
    return field;
  }

  //为加快查询的速度，而采用的缓冲的方式
  //程序内部在pdb_fileds表更新后会自动清除缓存,在后面的查询中会自动重初始化缓存.
  Vector vecFieldName=new Vector(0);
  public String GetFieldNameFromAlias(String table,String alias){
    byte[][] buf=null;
    String field = "";
    String strTmpTable ="";
    String strTmpAlias="";


    //初始化表
    if(vecFieldName==null || vecFieldName.size()==0){
      int[] fieldcount = new int[1];
      buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.PARA_FIELD_DESCRIBLE,
                             "DESCRIBE,TABLENAME,ALIAS", "", "", fieldcount, false);
      for(int i=0;i<buf.length;i++){
        vecFieldName.add(pdll.substr(buf[i]).trim());
      }
    }
    //执行查询
    for(int i=0;i<vecFieldName.size();i+=3){
      strTmpTable =  (String)vecFieldName.elementAt(i+1);
      strTmpAlias =  (String)vecFieldName.elementAt(i+2);
      if(strTmpTable.equalsIgnoreCase(table) && strTmpAlias.equalsIgnoreCase(alias)){
        field= (String)vecFieldName.elementAt(i);
        break;
      }
    }
    return field;
  }


  class jTreeSelecttionListener
      implements TreeSelectionListener {
    public void valueChanged(TreeSelectionEvent e) {
      jTreeSelection_valueChanged(e);
    }
  }

  class jMenuFileExitActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jMenuFileExit_actionPerformed(e);
    }
  }

  class jMenuOperateUnRevActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jMenuOperateUnRev_actionPerformed(e);
    }
  }

  class jMenuLoadAllRTDBActionLIstener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      pdll.reloadMode(0);
    }
  }

  class jMenuHelpAboutActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jMenuHelpAbout_actionPerformed(e);
    }
  }

  class jButtonLoginActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jButtonLogin_actionPerformed(e);
    }
  }

  class jButtonDeleteActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jButtonDelete_actionPerformed(e);
    }
  }

  class jButtonAddRecordActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jButtonAddRecord_actionPerformed(e);
    }
  }

  class jButtonSavaActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jButtonSave_actionPerformed(e);
    }
  }

  class jButtonLoadRDBActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jButtonLoadRDB_actionPerformed(e);
    }

  }

  class jButtonInsertRecordActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      jButtonInsertRecord_actionPerformed(e);
    }
  }
  //jButtonFindActionListener
  class jButtonFindActionListener
    implements ActionListener {
  public void actionPerformed(ActionEvent e) {
    jButtonFind_actionPerformed(e);
  }
}

  class jButtonOperActionListener
    implements ActionListener {
  public void actionPerformed(ActionEvent e) {
    jButtonOper_actionPerformed(e);
  }
}
class jButtonHelpActionListener
   implements ActionListener {
 public void actionPerformed(ActionEvent e) {
   jButtonHelp_actionPerformed(e);
 }
}
  class jButtonExeclctionListener
     implements ActionListener {
   public void actionPerformed(ActionEvent e) {
     jButtonExecl_actionPerformed(e);
   }
}
    class jButtonVerifyActionListener
            implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            try {
                jButtonVerify_actionPerformed(e);
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }

    class jButtonCopyActionListener
            implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            try {
                jButtonCopy_actionPerformed(e);
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }

    class jButtonDevCopyActionListener
            implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            try {
                jButtonDevCopy_actionPerformed(e);
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }


  //用于判断是否在框架上移动了鼠标
  class jTreeMouseMotionListener
      implements MouseMotionListener {
    public void mouseMoved(MouseEvent e) {
      ConstantDef.setLoginTime();
    };
    public void mouseDragged(MouseEvent e) {
    };
  }

  class jTabbedPanelChangeListener
      implements ChangeListener {
    public void stateChanged(ChangeEvent e) {
      jTabbedPanel_stateChanged(e);
    }
  }

  class jTabbedPanelMouseMotionListener
      implements MouseMotionListener {
    public void mouseMoved(MouseEvent e) {
      ConstantDef.setLoginTime();
    };
    public void mouseDragged(MouseEvent e) {
    };
  }

  class TimerActionListener
      implements ActionListener {
    public void actionPerformed(ActionEvent evt) {
      Timer_actionPerformed(evt);
    }

    public void Timer_actionPerformed(ActionEvent evt) {
      if (ConstantDef.getUserInstance().getEditPara() > 0) {
        if (Math.abs(ConstantDef.loginMinuteTime -
                     Calendar.getInstance().getTime().getTime() / (1000 * 60)) >
            ConstantDef.logoutTime) {
          ConstantDef.getUserInstance().initUser();
          ConstantDef.getUserInstance().setLoginF(false);
          jButtonLogin.setText("登录");
          //保留原来的浏览权限
          // ConstantDef.getUserInstance().setEditPara(0);
          // ConstantDef.getUserInstance().setMainman(0);
          // ConstantDef.getUserInstance().setGroup( -1);
          //ConstantDef.getUserInstance().setSuperdoor(0);
          updateShow();
          userstatusbar.setText("无登录用户");
          pdll.closedb(); //20071204
        }
      }
    }
  }

  public void jTreeSelection_valueChanged(TreeSelectionEvent e) {
    if (ConstantDef.getUserInstance().getBrowserPara() == 0) {
      return;
    }
    DefaultMutableTreeNode node = (DefaultMutableTreeNode) jTree1.
        getLastSelectedPathComponent();
    //hx  jtree-find
    if(findflag==true){
      f_node = (DefaultMutableTreeNode) jTree1.
          getLastSelectedPathComponent();
      findflag = true;
      findid = 0;
    }
    expandNodes(node);
    setRecordNums();
  }

  //File | Exit action performed
  public void jMenuFileExit_actionPerformed(ActionEvent e) {
    pdll.closedb();
    System.exit(0);
  }

  public void jMenuOperateUnRev_actionPerformed(ActionEvent e) {
    int tabno = jTabbedPane1.getSelectedIndex();
    jTree1.unRemove(tabno);
    setRecordNums();
  }

  //Help | About action performed
  public void jMenuHelpAbout_actionPerformed(ActionEvent e) {
    FrameDB_AboutBox dlg = new FrameDB_AboutBox(this);
    Dimension dlgSize = dlg.getPreferredSize();
    Dimension frmSize = getSize();
    Point loc = getLocation();
    dlg.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                    (frmSize.height - dlgSize.height) / 2 + loc.y);
    dlg.setModal(true);
    dlg.show();
  }

  void jButtonLogin_actionPerformed(ActionEvent e) { //用户登录
    User user = ConstantDef.getUserInstance();
    if (user.getLoginF()) {
      user.initUser();
      user.setLoginF(false);
      jButtonLogin.setText("登录");
      userstatusbar.setText("无登录用户");
      return;
    }
    UserLogin dialog = new UserLogin(this, "用户登录", true);
    dialog.show();
    updateShow();
    String operatordesc = "";
    if (user.getSuperdoor() != 0) {
      operatordesc = "调试用户";
      user.setLoginF(true);
      jButtonLogin.setText("注销");
    }
    else if (dialog.getCancelF()) {
      operatordesc = "无登录用户";
      user.setLoginF(false);
      jButtonLogin.setText("登录");
    }
    else {
      if (user.getGroupName().equalsIgnoreCase("") &&
          user.getGroupDesc().equalsIgnoreCase("") &&
          user.getDesc().equalsIgnoreCase("")) {
        operatordesc = "无登录用户";
        user.setLoginF(false);
        jButtonLogin.setText("登录");
      }
      else {
        operatordesc = "组名:" + user.getGroupName() +
            "   组别：" + user.getGroupDesc() +
            "   用户：" + user.getDesc();
        user.setLoginF(true);
        jButtonLogin.setText("注销");
      }
    }
    userstatusbar.setText(operatordesc);
  }

  void jButtonDelete_actionPerformed(ActionEvent e) { //删除
    int tabno = jTabbedPane1.getSelectedIndex();
    User user = ConstantDef.getUserInstance();
    String tablename = jTabbedPane1.getToolTipTextAt(tabno).trim();
    if (user.getDesc().equalsIgnoreCase("调度员")) {
      if (tablename.equalsIgnoreCase("SCD开关参数表") ||
          tablename.equalsIgnoreCase("SCD主变压器参数表") ||
          tablename.equalsIgnoreCase("SCD刀闸参数表") ||
          tablename.equalsIgnoreCase("SCD熔断器参数表") ||
          tablename.equalsIgnoreCase("SCD手车参数表") ||
          tablename.equalsIgnoreCase("SCD电容器参数表") ||
          tablename.equalsIgnoreCase("SCD电抗器参数表") ||
          tablename.equalsIgnoreCase("SCD消弧线圈参数表") ||
          tablename.equalsIgnoreCase("SCD互感器参数表") ||
          tablename.equalsIgnoreCase("SCD线路参数表") ||
          tablename.equalsIgnoreCase("SCD母线参数表") ||
          tablename.equalsIgnoreCase("SCD发电机参数表") ||
          tablename.equalsIgnoreCase("SCD负荷参数表") ||
          tablename.equalsIgnoreCase("SCD保护装置参数表") ||
          tablename.equalsIgnoreCase("SCD配电变压器参数表")) {
      }
      else {
        return;
      }
    }
    if (tabno >= 0) {
      int removerownums = jTree1.removeRow(tabno);
      if (removerownums > 0) {
        jTabbedPane1.setForegroundAt(tabno, Color.red);
        setRecordNums();
      }
    }
  }

  void jButtonAddRecord_actionPerformed(ActionEvent e) { //增加
    if (ConstantDef.getUserInstance().getEditPara() == 0) {
      return;
    }
    int tabno = jTabbedPane1.getSelectedIndex();
    User user = ConstantDef.getUserInstance();
    String tablename = jTabbedPane1.getToolTipTextAt(tabno).trim();
    if (user.getDesc().equalsIgnoreCase("调度员")) {
      if (tablename.equalsIgnoreCase("SCD开关参数表") ||
          tablename.equalsIgnoreCase("SCD主变压器参数表") ||
          tablename.equalsIgnoreCase("SCD刀闸参数表") ||
          tablename.equalsIgnoreCase("SCD熔断器参数表") ||
          tablename.equalsIgnoreCase("SCD手车参数表") ||
          tablename.equalsIgnoreCase("SCD电容器参数表") ||
          tablename.equalsIgnoreCase("SCD电抗器参数表") ||
          tablename.equalsIgnoreCase("SCD消弧线圈参数表") ||
          tablename.equalsIgnoreCase("SCD互感器参数表") ||
          tablename.equalsIgnoreCase("SCD线路参数表") ||
          tablename.equalsIgnoreCase("SCD母线参数表") ||
          tablename.equalsIgnoreCase("SCD发电机参数表") ||
          tablename.equalsIgnoreCase("SCD负荷参数表") ||
          tablename.equalsIgnoreCase("SCD保护装置参数表") ||
          tablename.equalsIgnoreCase("SCD配电变压器参数表")) {
      }
      else {
        return;
      }
    }
    if (tabno >= 0) {
      if(jTree1.addRow(tabno) >0 ) {
        jTabbedPane1.setForegroundAt(tabno, Color.red);
        setRecordNums();
      }
    }
  }

  void jButtonSave_actionPerformed(ActionEvent e) { //保存
    if (ConstantDef.getUserInstance().getEditPara() == 0) {
      return;
    }
    String hostname = "";
    try {
      InetAddress neta = InetAddress.getLocalHost();
      hostname = neta.getHostName();
    }
    catch (UnknownHostException nete) {
      hostname = "";
    }
    final DefaultMutableTreeNode node = (DefaultMutableTreeNode) jTree1.
        getLastSelectedPathComponent();
    String m_node = node.toString();
    int tabno = jTabbedPane1.getSelectedIndex();
    User user = ConstantDef.getUserInstance();
    String tablename = jTabbedPane1.getToolTipTextAt(tabno).trim();
    if (user.getDesc().equalsIgnoreCase("调度员")) {
      if (tablename.equalsIgnoreCase("SCD开关参数表") ||
          tablename.equalsIgnoreCase("SCD主变压器参数表") ||
          tablename.equalsIgnoreCase("SCD刀闸参数表") ||
          tablename.equalsIgnoreCase("SCD熔断器参数表") ||
          tablename.equalsIgnoreCase("SCD手车参数表") ||
          tablename.equalsIgnoreCase("SCD电容器参数表") ||
          tablename.equalsIgnoreCase("SCD电抗器参数表") ||
          tablename.equalsIgnoreCase("SCD消弧线圈参数表") ||
          tablename.equalsIgnoreCase("SCD互感器参数表") ||
          tablename.equalsIgnoreCase("SCD线路参数表") ||
          tablename.equalsIgnoreCase("SCD母线参数表") ||
          tablename.equalsIgnoreCase("SCD发电机参数表") ||
          tablename.equalsIgnoreCase("SCD负荷参数表") ||
          tablename.equalsIgnoreCase("SCD保护装置参数表") ||
          tablename.equalsIgnoreCase("SCD配电变压器参数表")) {
      }
      else {
        return;
      }
    }
    if (tabno >= 0) {
      String tbname = jTabbedPane1.getToolTipTextAt(tabno).trim();
      int saveflag = jTree1.saveChange(tabno, tbname);  //修改记录

      if (saveflag > 0) {
        if (Color.red == jTabbedPane1.getForegroundAt(tabno)) { //表被修改过
          if(PreProcessor.ALIAS){
            //重初始化字段
            if (tbname.trim().equalsIgnoreCase("pdb_field")) {
              vecFieldName.removeAllElements();
              vecFieldName.setSize(0);
            }
          }
          //delete for send fore cause rdbms error-200804
          /*
                    FertTable fert = new FertTable();
                    byte type = fert.getType(tbname);
                    if (type > 0) {
                      int[] fieldcount = new int[1];
                      byte[][] buf = new byte[0][0];
           buf = pdll.queryAnyTable(ConstantDef.MODELDB, "组参数表", "序号", "", "",
                                               fieldcount, false);
                      int i;
                      for (i = 0; i < buf.length; i++) {
                        int group = Integer.parseInt(pdll.substr(buf[i]));
           if (pdll.sendMsgToFert(group, type, tbname.getBytes()) >= 1) {
                          break;
                        }
                      }

                      if (i >= buf.length) {
                        String msg;
                        msg = "向前置机发送\"" + tbname + "\"修改信息失败!";
                      //  JOptionPane.showMessageDialog(null, msg, "发送信息提示",
                        //                              JOptionPane.ERROR_MESSAGE);
                      }
                    }
                    //delete for send fore cause rdbms error-200804
           */
          String tbnameEvt="";
          if (tbname.equalsIgnoreCase("SCD设备参数表") ||
              tbname.equalsIgnoreCase("遥信参数表") ||
              tbname.equalsIgnoreCase("遥测参数表") ||
              tbname.equalsIgnoreCase("电度参数表")) {
             tbnameEvt= m_node + " " + tbname;
          }

          if (!PreProcessor.JINHUA){
            pdll.sendEventMsg(user.getDesc().getBytes(), tbnameEvt.getBytes(),
                              hostname.getBytes());
          }

          //lcg 20090610 为给前置发送邮件修改
          FertTable fert = new FertTable();
          byte type = fert.getType(tbname);
          if (type > 0) {  //修改过，且需要发送通知邮件的表
            jTabbedPane1.setForegroundAt(tabno, Color.yellow);
              jButtonLoadRDB.setEnabled(true);
            return; //设置为黄色，并直接返回；
          }
        }
        if(jTabbedPane1.getForegroundAt(tabno)!=Color.yellow)
            jTabbedPane1.setForegroundAt(tabno, Color.black);
      }
    }
  }

  void jButtonLoadRDB_actionPerformed(ActionEvent e) { //加载实时库
    int tabno = jTabbedPane1.getSelectedIndex();
    User user = ConstantDef.getUserInstance();
    String tablename = jTabbedPane1.getToolTipTextAt(tabno).trim();
    if (user.getDesc().equalsIgnoreCase("调度员")) {
      if (tablename.equalsIgnoreCase("SCD开关参数表") ||
          tablename.equalsIgnoreCase("SCD主变压器参数表") ||
          tablename.equalsIgnoreCase("SCD刀闸参数表") ||
          tablename.equalsIgnoreCase("SCD熔断器参数表") ||
          tablename.equalsIgnoreCase("SCD手车参数表") ||
          tablename.equalsIgnoreCase("SCD电容器参数表") ||
          tablename.equalsIgnoreCase("SCD电抗器参数表") ||
          tablename.equalsIgnoreCase("SCD消弧线圈参数表") ||
          tablename.equalsIgnoreCase("SCD互感器参数表") ||
          tablename.equalsIgnoreCase("SCD线路参数表") ||
          tablename.equalsIgnoreCase("SCD母线参数表") ||
          tablename.equalsIgnoreCase("SCD发电机参数表") ||
          tablename.equalsIgnoreCase("SCD负荷参数表") ||
          tablename.equalsIgnoreCase("SCD保护装置参数表") ||
          tablename.equalsIgnoreCase("SCD配电变压器参数表")) {
      }
      else {
        return;
      }
    }
    if (tabno >= 0) {
      String tb = jTabbedPane1.getToolTipTextAt(tabno).trim();
      if (tb.equalsIgnoreCase("dbinfoini")) {
        byte[][] logtimelong = pdll.querydbinfoini("项目名='注销登录时限'");
        if (logtimelong.length > 0) {
          ConstantDef.logoutTime = Integer.parseInt(pdll.substr(logtimelong[0]));
        }
        return;
      }
      if (tb.equalsIgnoreCase("web事件管理表")) {
        pdll.sendMsgToWeb();
        return;
      }
      Vector vname = new Vector(0);
      pdll.getRelatedLoadTable(ConstantDef.MODELDB, tb, vname);
      if (vname.size() > 0) {
        Vector name = new Vector(0);
        Vector srvname = new Vector(0);
        name = (Vector) vname.elementAt(0); //表名
        srvname = (Vector) vname.elementAt(1); //服务器名
        byte[][] namepara = new byte[name.size()][0];
        byte[][] srvnamepara = new byte[name.size()][0];
        for (int i = 0; i < name.size(); i++) {
          namepara[i] = name.elementAt(i).toString().getBytes();
          srvnamepara[i] = srvname.elementAt(i).toString().getBytes();
        }
        int rtn = 0;
        //专为DA加载实时库用，有先后顺序要求
        if (name.elementAt(0).toString().equalsIgnoreCase("DA节点参数表")
            || name.elementAt(0).toString().equalsIgnoreCase("DA开关参数表")) {
          for (int j = 0; j < name.size(); j++) {
            byte[][] namebyte = new byte[1][0];
            byte[][] srvnamebyte = new byte[1][0];
            namebyte[0] = name.elementAt(j).toString().getBytes();
            srvnamebyte[0] = srvname.elementAt(j).toString().getBytes();
            rtn = rtn + pdll.reCall(srvnamebyte, namebyte);
            try {
              Thread.sleep(3000);
            }
            catch (InterruptedException ee) {
              // ee.printStackTrace();
            }
          }
        }
        else {

          rtn = pdll.reCall(srvnamepara, namepara);
//            jButtonLoadRDB.setEnabled(false);
//            try {
//                Thread.sleep(1000);
//            }
//            catch (InterruptedException ee) {
//                // ee.printStackTrace();
//            }
            jButtonLoadRDB.setEnabled(false);
        }
        if (rtn <= 0) {
          JOptionPane.showMessageDialog(null, "加载\"" + tb + "\"相关的实时库表失败!",
                                        "加载实时库提示", JOptionPane.ERROR_MESSAGE);
        }
      }
      else {
        JOptionPane.showMessageDialog(null, "\"" + tb + "\"没有相关的实时库表",
                                      "加载实时库提示",
                                      JOptionPane.INFORMATION_MESSAGE);
      }
    }


    //发送通知邮件给前置更新表
    for (int i = 0; i < jTabbedPane1.getTabCount(); i++) {
      if(jTabbedPane1.getForegroundAt(i)!=Color.yellow){
        continue;
      }
      jTabbedPane1.setForegroundAt(i,Color.black);
      String tabName = jTabbedPane1.getToolTipTextAt(i).trim();
      FertTable fert = new FertTable();
      byte type = fert.getType(tabName);
      if (type > 0) {
        int[] fieldcount = new int[1];
        byte[][] buf = new byte[0][0];
        buf = pdll.queryAnyTable(ConstantDef.MODELDB, "组参数表", "序号", "", "",
                                 fieldcount, false);
        int j;
        for (j = 0; j < buf.length; j++) {
          int group = Integer.parseInt(pdll.substr(buf[j]));
          if (pdll.sendMsgToFert(group, type, tabName.getBytes()) >= 1) {

          }
          try {
            Thread.sleep(500);
          }
          catch (InterruptedException ex) {
          }
          if (j >= buf.length) {
            String msg;
            msg = "向前置机发送\"" + tabName + "\"修改信息失败!";
            //  JOptionPane.showMessageDialog(null, msg, "发送信息提示",
            //                              JOptionPane.ERROR_MESSAGE);
          }
        }
      }
    }

  }

  void jButtonOper_actionPerformed(ActionEvent e) {
    TextFileViewer filedialog = new TextFileViewer(this, "操作记录", true);
    filedialog.show();

  }

  void jButtonHelp_actionPerformed(ActionEvent e) {
    try {
      Runtime process = Runtime.getRuntime();
      String str = Dlllink.runhome + "\\help\\D6000智能配电网主站系统-维护说明书.chm::/_36.htm";
      process.exec("hh.exe " + str);
    }
    catch (IOException ex) {
    //  System.out.println("framedb.java");
    }

  }

  //导出当前jtable的数据到excel
  void jButtonExecl_actionPerformed(ActionEvent e) {
    int tabindex = jTabbedPane1.getSelectedIndex();
    String tablename = "";
    String filename=  Dlllink.runhome + "/excel/";
    File f=new File(filename);
    if(f.exists()==false){
      f.mkdir();
    }


    MyJTable tempjtable;
    if (tabindex < 0) {
      return;
    }
    if (tabindex >= 0) {
      jTree1.m_curtablename = jTabbedPane1.getTitleAt(tabindex);
      tablename = jTree1.m_curtablename;
      int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.elementAt(
          tabindex).toString().trim());
      MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(tabindex);

      if (tabnocount > 1) {
        JTabbedPane jtabbedPaneTabno = (JTabbedPane) jTree1.vTabnoJTab.
            elementAt(tabindex);
        JTableToExcel toexcel = new JTableToExcel();
        tablename = toexcel.replaceStr(jTree1.m_curtablename, "/", "-");
        filename = Dlllink.runhome + "/excel/" + tablename + ".xls";
        File fileobject = new File(filename);
        if (fileobject.exists()) {
          fileobject.delete();
        }

        toexcel.export(fileobject, tablename, jtabbedPaneTabno.getTitleAt(0),
                       "", myjtable);
        for (int i = 1; i < tabnocount; i++) {
          tempjtable = myjtable.getJTable(i - 1);
          if (tempjtable == null) {
            return;
          }
          toexcel.export(fileobject, tablename, jtabbedPaneTabno.getTitleAt(i),
                         "", tempjtable);

        }
      }
      else {
        JTableToExcel toexcel = new JTableToExcel();
        tablename = toexcel.replaceStr(jTree1.m_curtablename, "/", "-");
        filename = Dlllink.runhome + "/excel/" + tablename + ".xls";
        File fileobject = new File(filename);
        if(fileobject.exists()){
          fileobject.delete();
        }
        toexcel.export(fileobject, tablename, tablename, "", myjtable);
      }
    }
    JOptionPane.showMessageDialog(null, "导出["+ filename +"]成功", "导出文件",
                                    JOptionPane.CLOSED_OPTION);
  }

    //数据库相关表的校验 hx
    void jButtonVerify_actionPerformed(ActionEvent e) throws IOException {

        VerifyProgressBar verifyProgressBar = new VerifyProgressBar(ApplicationDB.frameDB, "模型校验", true);

        verifyProgressBar.setModal(true);
        verifyProgressBar.setLocationRelativeTo(null);
        verifyProgressBar.show();
        verifyProgressBar.pack();
    }

    //站所复制  add by hongxiang
    void jButtonCopy_actionPerformed(ActionEvent e) throws IOException {

        BayCopyDialog bayCopyDialog = new BayCopyDialog(ApplicationDB.frameDB, "站所复制", true);

        bayCopyDialog.setModal(true);
        bayCopyDialog.setLocationRelativeTo(null);
        bayCopyDialog.show();
        bayCopyDialog.pack();
    }

    //设备复制  add by hongxiang
    void jButtonDevCopy_actionPerformed(ActionEvent e) throws IOException {

        DevCopyDialog devCopyDialog = new DevCopyDialog(ApplicationDB.frameDB, "设备复制", true);

        devCopyDialog.setModal(true);
        devCopyDialog.setLocationRelativeTo(null);
        devCopyDialog.show();
        devCopyDialog.pack();
    }

//jButtonFind_actionPerformed
  void jButtonFind_actionPerformed(ActionEvent e) { //在jtree中查询数据
    String str = FindTxt.getText();
    if (f_node == null)
      return;
    if (f_node.isLeaf())
      return;
    if (str == null)
      return;
    TreePath treePath = new TreePath(f_node);
    treePath = findInPath(treePath, str);
    if (treePath != null) {
      findflag=false;
      jTree1.setSelectionPath(treePath);
      findflag=true;
      jTree1.scrollPathToVisible(treePath);
    }

  }

  private TreePath findInPath(TreePath treePath, String str) {
    String value;
    Object object = treePath.getLastPathComponent();
    if (object == null) {
      return null;
    }
      TreeModel model = jTree1.getModel();
      int n = model.getChildCount(object);
      if(findid == n){
        findid=0;
      }
      for (int i = findid; i < n; i++) {
        Object child = model.getChild(object, i);
        TreePath path = treePath.pathByAddingChild(child);
        value=child.toString().toLowerCase();
        str=str.toLowerCase();
        int find = value.indexOf(str);
        if (find != -1 && !str.equals("")) {
          findid = i+1;
          return path;

        }
      }
      JOptionPane.showMessageDialog(null, "查找结束", "提示",
                                          JOptionPane.ERROR_MESSAGE);
      return null;

  }

  void jButtonInsertRecord_actionPerformed(ActionEvent e) { //插入记录
    if (ConstantDef.getUserInstance().getEditPara() == 0) {
      return;
    }
    int tabno = jTabbedPane1.getSelectedIndex();
    User user = ConstantDef.getUserInstance();
    String tablename = jTabbedPane1.getToolTipTextAt(tabno).trim();
    if (user.getDesc().equalsIgnoreCase("调度员")) {
      if (tablename.equalsIgnoreCase("SCD开关参数表") ||
          tablename.equalsIgnoreCase("SCD主变压器参数表") ||
          tablename.equalsIgnoreCase("SCD刀闸参数表") ||
          tablename.equalsIgnoreCase("SCD熔断器参数表") ||
          tablename.equalsIgnoreCase("SCD手车参数表") ||
          tablename.equalsIgnoreCase("SCD电容器参数表") ||
          tablename.equalsIgnoreCase("SCD电抗器参数表") ||
          tablename.equalsIgnoreCase("SCD消弧线圈参数表") ||
          tablename.equalsIgnoreCase("SCD互感器参数表") ||
          tablename.equalsIgnoreCase("SCD线路参数表") ||
          tablename.equalsIgnoreCase("SCD母线参数表") ||
          tablename.equalsIgnoreCase("SCD发电机参数表") ||
          tablename.equalsIgnoreCase("SCD负荷参数表") ||
          tablename.equalsIgnoreCase("SCD保护装置参数表") ||
          tablename.equalsIgnoreCase("SCD配电变压器参数表")) {
      }
      else {
        return;
      }
    }
    if (tabno >= 0) {
      if(jTree1.insertRow(tabno) > 0){
        jTabbedPane1.setForegroundAt(tabno, Color.red);
        setRecordNums();
      }
    }
  }

  public void jTabbedPanel_stateChanged(ChangeEvent e) {
    int tabindex = jTabbedPane1.getSelectedIndex();
    if (tabindex < 0) {
      return;
    }
    infoBar.setText(jTree1.vTitle.get(tabindex).toString());

    if (tabindex >= 0) {
      jTree1.m_curtablename = jTabbedPane1.getTitleAt(tabindex);
      int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.elementAt(
          tabindex).toString().trim());
      MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(tabindex);
      if (tabnocount > 1) {
        JTabbedPane jtabbedPaneTabno = (JTabbedPane) jTree1.vTabnoJTab.
            elementAt(tabindex);
        int index2 = jtabbedPaneTabno.getSelectedIndex();
        if (index2 >= 1) {
          myjtable = myjtable.getJTable(index2 - 1);
        }
      }
      if (myjtable == null) {
        return;
      }
      String tabletype = myjtable.getTableType();
      if (tabletype.equalsIgnoreCase("U")) {
        if (ConstantDef.getUserInstance().getEditPara() > 0) {
          setButton(true);
        }
      }
      else {
        setButton(false);
      }
      ConstantDef.setLoginTime();
    }
    else
      jTree1.m_curtablename = "";
    setRecordNums();
    try {
      jScrollPane1Tree.updateUI();
    }
    catch (NullPointerException ne) {

    }
  }

  //代码检查
  void jMenuCheckCode_actionPerformed(ActionEvent e) {
    int tabindex = jTabbedPane1.getSelectedIndex();
    if (tabindex < 0)
      return;
    int tabnocount = Integer.parseInt(jTree1.vTableTabnoCount.elementAt(
        tabindex).toString().trim());
    MyJTable myjtable = (MyJTable) jTree1.vListTable.elementAt(tabindex);
    MyTableModel mytablemodel = (MyTableModel) myjtable.getModel();

    String curTableName = jTabbedPane1.getToolTipTextAt(tabindex);
    if (!curTableName.equalsIgnoreCase(ConstantDef.YXPARATABLE) &&
        !curTableName.equalsIgnoreCase(ConstantDef.YCPARATABLE))
      return;
    MyTableModel devcodeModel = null, typeModel = null, codeModel = null;
    int devcodeCol = -1, typeCol = -1, codeCol = -1;
    MyJTable tabtmp;
    MyTableModel modeltmp;
    for (int j = 0; j < tabnocount; j++) {
      if (j == 0) {
        tabtmp = myjtable;
        modeltmp = mytablemodel;
      }
      else {
        tabtmp = myjtable.getJTable(j - 1);
        modeltmp = (MyTableModel) tabtmp.getModel();
      }

      for (int i = 0; i < modeltmp.getColumnCount(); i++) {
        String fieldname = modeltmp.getColumnName(i);
        if (fieldname.equalsIgnoreCase("代码")) {
          codeModel = modeltmp;
          codeCol = i;
        }
        else if (fieldname.equalsIgnoreCase("类型")) {
          typeModel = modeltmp;
          typeCol = i;
        }
        else if (fieldname.equalsIgnoreCase("设备代码")) {
          devcodeModel = modeltmp;
          devcodeCol = i;
        }
      }
    }

    if (devcodeModel == null || typeModel == null || codeModel == null) {
      JOptionPane.showMessageDialog(this, "没有进行校验");
      return;
    }
    ReadDB readdb = new ReadDB(ConstantDef.MODELDB, "SCD对象类型表");
    readdb.queryTable("类型代码,类型值", "", "");
    int rows = readdb.getRecords();
    String postStr = "";
    for (int i = 0; i < mytablemodel.getRowCount(); i++) {
      postStr = "";
      String stype = typeModel.getValueAt(i, typeCol).toString();
      String sdevcode = devcodeModel.getValueAt(i, devcodeCol).toString();
      String scode = codeModel.getValueAt(i, codeCol).toString();
      for (int j = 0; j < rows; j++) {
        if (stype.equalsIgnoreCase(readdb.getAFieldValue(j, 1))) {
          postStr = readdb.getAFieldValue(j, 0);
          break;
        }
      }
      String comCode = sdevcode + postStr;
      if (comCode.compareTo(scode) != 0) {
        mytablemodel.managemodel.setCodeErr(i);
      }
    }

  }
}

class jButtonLoadRDBActionListener
    implements java.awt.event.ActionListener {
  private FrameDB adaptee;

  jButtonLoadRDBActionListener(FrameDB adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButtonLoadRDB_actionPerformed(e);
  }
}
