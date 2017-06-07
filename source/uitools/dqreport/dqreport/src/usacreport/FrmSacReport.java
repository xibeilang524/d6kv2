package usacreport;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import com.f1j.swing.*;
import com.f1j.ss.*;
import javax.swing.filechooser.*;
import com.f1j.util.F1Exception;
import java.io.*;
import javax.swing.border.*;
import java.util.Vector;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import java.awt.Dimension;
import java.awt.Font;
import javax.swing.tree.DefaultTreeCellRenderer;
import java.awt.BorderLayout;
import javax.swing.tree.TreeCellRenderer;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class FrmSacReport
    extends JFrame {
   private JPanel contentPane;

   JSplitPane jSplitPane1 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
   public static BorderLayout borderLayout = new BorderLayout();
   public static JPanel jBookpanel = new JPanel(borderLayout);
   public static int flag = 0;
   private JMenuBar jMenuSave = new JMenuBar();
   private JMenu jMenuFile = new JMenu();

   private JMenuItem jMenuFileExit = new JMenuItem();
   private JMenu jMenuHelp = new JMenu();
   private JMenuItem jMenuHelpAbout = new JMenuItem();
   private JButton jButdel = new JButton();
   private JToolBar jToolBar = new JToolBar();
   private JButton jButopen = new JButton();
   private JButton jButsave = new JButton();
   private JButton jButMerge = new JButton();
   private ImageIcon imagetitle;
   private ImageIcon imageopen;
   private ImageIcon imagesave;
   private ImageIcon imagehelp;
   private ImageIcon imagenew;
   private ImageIcon imageexit;
   private ImageIcon imagecut;
   private ImageIcon imagecopy;
   private ImageIcon imagepaste;
   private ImageIcon imageprint;
   private ImageIcon imageprintview;
   private ImageIcon imagehtml;
   private ImageIcon imageexcel;
   private ImageIcon imageredo;
   private ImageIcon imageundo;
   private ImageIcon imagego;
   private ImageIcon imagecheck;
   private ImageIcon imageusr;
   private ImageIcon imagemerge;
   //---------------------------
   private ImageIcon imagerefresh;
   private ImageIcon imagesql;
   private ImageIcon imageyc;
   private ImageIcon imageyx;
   private ImageIcon imagertdb;
   private ImageIcon imagedes;
   private ImageIcon imageupdate;
   private ImageIcon imageload;
   private ImageIcon imageprintarea;
   private ImageIcon imageclose;
   //---------------------------
   private ImageIcon imageopen16;
   private ImageIcon imagenew16;
   private ImageIcon imagesave16;
   private ImageIcon imagesaveas16;
   private ImageIcon imageprint16;
   private ImageIcon imageprintset16;
   private ImageIcon imageprintn16;
   private ImageIcon imageexcel16;
   private ImageIcon imagexml16;
   private ImageIcon imageexit16;
   private ImageIcon imageb;
   private ImageIcon imageprintview16;

    //管理菜单加图标 ---------------------------
    private ImageIcon reportdb16; //报表数据库管理
    private ImageIcon treenode16; //报表数据库管理

    // 查询菜单加图标 ---------------------------
    private ImageIcon paraquery16; //参数查询
    private ImageIcon eventquery16; //事项查询

    // 工具菜单加图标 ---------------------------
    private ImageIcon maxarea16; //设置最大显示区域
    private ImageIcon rowset16; //行设置
    private ImageIcon columnset16; //列设置
    private ImageIcon stoptimeprint16; //停止定时打印
    private ImageIcon grid16; //显示/隐藏网格
    private ImageIcon equalto16; //等差填充
    private ImageIcon formula16; //常用公式
    private ImageIcon plus16; //正值显示
    private ImageIcon lock16; //锁定行列
    private ImageIcon insert16; //插入工作表
    private ImageIcon deletethistable16; //删除当前表
    private ImageIcon insertchart16; //插入图表
    private ImageIcon datafill16; //规律数据填充
    private ImageIcon datasetting16; //数据整定
    private ImageIcon deaddata16; //死数查询

   //--------------------------
   private JLabel statusBar = new JLabel();
   private BorderLayout borderLayout1 = new BorderLayout();
   public static JBook jBook1 = new JBook();
   //public static JBook jBook2 = new JBook();
   private JMenuItem jMenuNew = new JMenuItem();
   private JMenuItem jMenuISave = new JMenuItem();
   private JMenuItem jMenuOpen = new JMenuItem();
   //public static com.sac.dbinfo.Dlllink  f1jdll = com.sac.dbinfo.Dlllink.getInstance() ;
   public static usacreport.dllf1j f1jdll = usacreport.dllf1j.getInstance();
   public static usacreport.dlgSelect dlgsel = null;
   private usacreport.dlgFill dlgfill = null;
   private usacreport.dlgFillK dlgFillK = null;
   private usacreport.dlgFillSQL dlgFillSQL = null;

   private usacreport.dlgfillrtdb dlgfillrtdb=null;   //实时数据填充窗口

   private usacreport.dlgPatchPrint dlgPatchPrint = null;
   private usacreport.dlgfillshow dlgshow = null;
   private usacreport.dlgusrmanage dlgusrman = null;
   private usacreport.dlgusr dlgusr1 = null;
   public static usacreport.dlgtreem dlgtreem1 = null;
   private usacreport.dlgpara dlgpara1 = null;
   private usacreport.dlgEvent dlgEvent1 = null;
   private usacreport.dlgfillgl dlgfillgl1 = null;
   private usacreport.dlgPringpara dlgprintpara1 = null;
   private usacreport.dlgModData dlgmoddata1 = null;
   private usacreport.dlgdeadata dlgdeadatal = null;
   private usacreport.dlgfillfh dlgfillfh1 = null;
   private usacreport.dlgExport dlgExport1 = null;
   private usacreport.dlgPrint dlgPrint1 = null;
   public static usacreport.dlgModP dlgmodp1 = null;
   public static String s_sacenv = "";
   public static String s_seprate = "";
   private JButton jButFx = new JButton();
   private JButton jButFk = new JButton();
   private JButton jButSQL = new JButton();
   private JButton jButRTDB = new JButton();
   private JMenuItem jMenuhtml = new JMenuItem();
   public static String g_filename = "";
   private JButton jButFx1 = new JButton();
   private JMenu jMenu2 = new JMenu();
   private JMenuItem jMenuchangeusr = new JMenuItem();
   private JMenu jMenu3 = new JMenu();
   public static int g_role = -1;
   private JMenuItem jMenuexcel = new JMenuItem();
   private JMenuItem jMenusomexls = new JMenuItem();
   private TitledBorder titledBorder1;
   private JButton jButnew = new JButton();
   public static int g_gridrateu = 3;
   public static int g_gridrated = 2;
   private JMenuItem jMenuIpara = new JMenuItem();
   private boolean bZoom = true;
   private JMenuItem jMenuPrint = new JMenuItem();
   private JMenuItem jMenuPrintPatch = new JMenuItem();
   private JMenuItem jMenuPrintSome = new JMenuItem();
   private JMenuItem jMenubbman = new JMenuItem();
   //hongxiang添加打印预览
   private JMenuItem jMenuPrintView = new JMenuItem();

   private JButton jButpaste = new JButton();
   private JButton jButcopy = new JButton();
   private JButton jButcut = new JButton();
   private JButton jButPrint = new JButton();

   public static Vector vectorpara = new Vector();
   private JButton jButMod = new JButton();
   public static String g_strsdate = "";
   public static boolean g_savemode = true;
   private JMenu jMenu4 = new JMenu();
   private JMenu jMenu5 = new JMenu();
   private JMenuItem jAddRow = new JMenuItem();
   private JMenuItem jDelRow = new JMenuItem();
   private JMenuItem jMenuSetRowNums = new JMenuItem();
   private JMenuItem jAddCol = new JMenuItem();
   private JMenuItem jDelCol = new JMenuItem();
   private JMenuItem jMenuSetColNums = new JMenuItem();
   private JMenuItem jMenuevent = new JMenuItem();
   private JMenuItem jMenuItPrint = new JMenuItem();
   private JMenuItem jMenugrid = new JMenuItem();
   private JMenuItem jMenuIRowH = new JMenuItem();
   private JMenuItem jMenuColwidth = new JMenuItem();
   private JMenu jMenu6 = new JMenu();
   private JMenuItem jMenuPArea = new JMenuItem();
   private JMenuItem jMenuSArea = new JMenuItem();
   public static JTree jTreetab;
   public static JScrollPane jScrollTree = new JScrollPane();

   private JMenuItem jMenutest = new JMenuItem();
   private JMenuItem jMenufillE = new JMenuItem();
   private JMenuItem jMenuSaveAs = new JMenuItem(); //= new JTree();
   public static String g_newfilename = "新报表";
   private JMenu jMenu7 = new JMenu();
   private JMenuItem jMenumax = new JMenuItem();
   private JMenuItem jMenumin = new JMenuItem();
   private JMenuItem jMenuave = new JMenuItem();
   private  JButton jButPA = new JButton();
   private  JButton jButFh = new JButton();
   public static JComboBox jComPSize = new JComboBox();
   private JMenu jMenu1 = new JMenu();
   private JMenuItem jMenutreeM = new JMenuItem();
   private JMenu jMenu8 = new JMenu();
   public static int h_row = 0, h_col = 0, z_row = 0, z_col = 0;
   public static double A3_h = 29.7, A3_w = 42.0, A4_h = 21.0, A4_w = 29.7;
   public static boolean b_run = false;
   public static Vector g_veczsxxid = new Vector();
   public static Vector g_veczsxxdec = new Vector();
   private int pre_key = 0; //
   private double rate_zoom = 1.0;
   private boolean zoom_first = true;
   public static boolean babs = false;
   public static  int reportflag=0;
   int i = 1;
   //Construct the frame
   public FrmSacReport() {
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);
      try {
         jbInit();
      }
      catch (Exception e) {
         e.printStackTrace();
      }
   }

   //Component initialization
   private void jbInit() throws Exception {

      imageclose=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/close.png"));
      imagetitle=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/dqreport.png"));
      imageopen = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/open.png"));
      imagesave = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/save.png"));
      imagehelp = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/help.gif"));
      imagenew = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/new.png"));
      imageexit = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/exit.gif"));
      imagecut = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/cut.png"));
      imagecopy = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/copy.png"));
      imagepaste = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/paste.png"));
      imageprint = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/print.png"));
      imageprintview = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/printview.jpg"));
      imagehtml = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/html.jpg"));
      imageexcel = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/excel.gif"));
      imageredo = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/redo16.gif"));
      imageundo = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/undo16.gif"));
      imagego = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/go.gif"));
      imageusr = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/usr.png"));
      imagemerge = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/merge.png"));
      imagerefresh=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/refresh.png"));
      imagesql=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/sql.png"));
      imagertdb=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/rtdb.png"));
      imageyc=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/yc.png"));
      imageyx=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/yx.png"));
      imagedes=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/des.png"));
      imageupdate=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/update.png"));
      imageprintarea=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/printarea.png"));
      imageload=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/load.png"));

      imageopen16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/open16.png"));
      imagenew16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/new16.png"));
      imagesave16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/save16.png"));
      imagesaveas16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/saveas16.png"));
      imageprint16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/print16.png"));
      imageprintset16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/printset16.png"));
      imageprintn16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/printn16.png"));
      imageexcel16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/excel16.png"));
      imagexml16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/xml16.png"));
      imageexit16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/exit16.png"));
      imageprintview16=new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/printview.png"));

       //管理菜单图标
       reportdb16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/reportdb16.png"));
       treenode16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/treenode16.png"));

       //查询菜单图标
       paraquery16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/paraquery16.png"));
       eventquery16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/eventquery16.png"));

       //工具菜单图标
       maxarea16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/maxarea16.png"));
       rowset16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/rowset16.png"));
       columnset16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/columnset16.png"));
       stoptimeprint16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/stoptimeprint16.png"));
       grid16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/grid16.png"));
       equalto16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/equalto16.png"));
       formula16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/formula16.png"));
       plus16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/plus16.png"));
       lock16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/lock16.png"));
       insert16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/insert16.png"));
       deletethistable16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/deletethistable16.png"));
       insertchart16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/insertchart16.png"));
       datafill16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/datafill16.png"));
       datasetting16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/datasetting16.png"));
       deaddata16 = new ImageIcon(usacreport.FrmSacReport.class.getResource("icon/deaddata16.png"));

      contentPane = (JPanel)this.getContentPane();
      contentPane.setLayout(borderLayout1);
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
       //设置登录最大化
       this.setExtendedState(Frame.MAXIMIZED_BOTH);
      try {
         this.setSize(new Dimension(screenSize.width * 4 / 5, screenSize.height * 4 / 5));
      }
      catch (java.lang.NullPointerException enull) {}
      this.setTitle("D6000报表工具");
      this.setIconImage(imagetitle.getImage());
      statusBar.setText(" ");
      jMenuFile.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuFile.setText("文件(F)");
      jMenuFileExit.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuFileExit.setIcon(imageexit16);
      jMenuFileExit.setText("退出");
      jMenuFileExit.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuFileExit_actionPerformed(e);
         }
      });
      jMenuHelp.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuHelp.setText("帮助(H)");
      jMenuHelpAbout.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuHelpAbout.setIcon(imagehelp);
      jMenuHelpAbout.setText("关于");
      jMenuHelpAbout.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuHelpAbout_actionPerformed(e);
         }
      });
      jButopen.setMnemonic('0');
      jButopen.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButopen_actionPerformed(e);
         }
      });
      jButopen.setMaximumSize(new Dimension(40, 37));
      jButopen.setPreferredSize(new Dimension(71, 25));
      jButopen.setToolTipText("打开");
      jButopen.setIcon(imageopen);
      jButsave.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButsave_actionPerformed(e);
         }
      });
      jButsave.setMaximumSize(new Dimension(40, 37));
      jButsave.setToolTipText("保存");
      jButsave.setIcon(imagesave);
      jMenuNew.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuNew.setIcon(imagenew16);
      jMenuNew.setText("新 建");
      jMenuNew.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuNew_actionPerformed(e);
         }
      });
      jMenuISave.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuISave.setIcon(imagesave16);
      jMenuISave.setText("保 存");
      jMenuISave.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuISave_actionPerformed(e);
         }
      });
      jMenuOpen.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuOpen.setIcon(imageopen16);
      jMenuOpen.setText("打 开");
      jMenuOpen.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuOpen_actionPerformed(e);
         }
      });
      jButFx.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButFx.setMaximumSize(new Dimension(40, 37));
      jButFx.setMinimumSize(new Dimension(40, 37));
    //  jButFx.setPreferredSize(new Dimension(51, 27));
      jButFx.setToolTipText("遥测填充");
      jButFx.setIcon(imageyc);
      jButFx.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButFx_actionPerformed(e);
         }
      });
      jButFk.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButFk.setMaximumSize(new Dimension(40, 37));
      jButFk.setMinimumSize(new Dimension(40, 37));
      jButFk.setToolTipText("遥信填充");
      jButFk.setIcon(imageyx);
      jButFk.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButFk_actionPerformed(e);
         }
      });

      jButdel.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButdel.setMaximumSize(new Dimension(40, 37));
      jButdel.setMinimumSize(new Dimension(40, 37));
      jButdel.setToolTipText("关闭报表");
      jButdel.setIcon(imageclose);
      jButdel.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButdel_actionPerformed(e);
         }
      });

      jButSQL.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButSQL.setMaximumSize(new Dimension(40, 37));
      jButSQL.setMinimumSize(new Dimension(40, 37));
      jButSQL.setToolTipText("SQL填充");
      jButSQL.setIcon(imagesql);
      jButSQL.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButSQL_actionPerformed(e);
         }
      });
      jButRTDB.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButRTDB.setMaximumSize(new Dimension(40, 37));
      jButRTDB.setMinimumSize(new Dimension(40, 37));
      jButRTDB.setToolTipText("实时数据填充");
      jButRTDB.setIcon(imagertdb);
      jButRTDB.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButRTDB_actionPerformed(e);
         }
      });


      jMenuhtml.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuhtml.setIcon(imagexml16);
      jMenuhtml.setText("输出html");
      jMenuhtml.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuhtml_actionPerformed(e);
         }
      });
      jButFx1.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButFx1_actionPerformed(e);
         }
      });

     // jButFx1.setText("Fx(显示)");
      jButFx1.setToolTipText("单元格描述");
      jButFx1.setActionCommand("Fx(属性)");
      jButFx1.setIcon(imagedes);
      jButFx1.setPreferredSize(new Dimension(40, 37));
      jButFx1.setMinimumSize(new Dimension(40, 37));
      jButFx1.setFont(new java.awt.Font("Dialog", 0, 12));
      jButFx1.setMaximumSize(new Dimension(40, 37));
      jMenu2.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu2.setText("用户(U)");
      jMenuchangeusr.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuchangeusr.setIcon(imageusr);
      jMenuchangeusr.setText("更改用户");
      jMenuchangeusr.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuchangeusr_actionPerformed(e);
         }
      });
      jMenu3.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu3.setText("工具(T)");
      jMenuexcel.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuexcel.setIcon(imageexcel16);
      jMenuexcel.setText("输出excel");
      jMenuexcel.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuexcel_actionPerformed(e);
         }
      });
      jMenusomexls.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenusomexls.setIcon(imageexcel);
      jMenusomexls.setText("批量输出excel");
      jMenusomexls.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenusomexls_actionPerformed(e);
         }
      });

      jToolBar.setFont(new java.awt.Font("Dialog", 0, 12));
      jToolBar.setBorder(null);
      jToolBar.setDebugGraphicsOptions(0);
      jToolBar.setToolTipText(""); // jButtonHelp.setIcon(null);

      jButnew.setMaximumSize(new Dimension(40, 37));
      jButnew.setMinimumSize(new Dimension(79, 35));
       jButnew.setPreferredSize(new Dimension(40, 25));
       jButnew.setToolTipText("新建");
       jButnew.setIcon(imagenew);
       jButnew.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jButnew_actionPerformed(e);
           }
       });
       jMenuIpara.setFont(new java.awt.Font("Dialog", 0, 12));
       jMenuIpara.setText("参数查询");
       jMenuIpara.setIcon(paraquery16);
       jMenuIpara.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuIpara_actionPerformed(e);
         }
      });
      jMenuPrint.setIcon(imageprint16);
      jMenuPrint.setText("打印");
      jMenuPrint.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuPrint_actionPerformed(e);
         }
      });
      //hongxiang 打印预览处理

      jMenuPrintView.setIcon(imageprintview16);
      jMenuPrintView.setText("打印预览");
      jMenuPrintView.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuPrintView_actionPerformed(e);
         }
      });

      jMenuPrintPatch.setText("批量打印");
      jMenuPrintPatch.setIcon(imageprintn16);
      jMenuPrintPatch.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuPrintPatch_actionPerformed(e);
         }
      });
      jMenuPrint.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jMenuPrintPatch.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jMenuPrintView.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jMenuSaveAs.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));

      jMenuPrintSome.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuPrintSome.setIcon(imageprint);
      jMenuPrintSome.setText("批量打印");
      jMenuPrintSome.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuprintsome_actionPerformed(e);
         }
      });
      jMenubbman.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenubbman.setText("报表数据库管理");
       jMenubbman.setIcon(reportdb16);
      jMenubbman.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenubbman_actionPerformed(e);
         }
      });
      jButpaste.setIcon(imagepaste);
      jButpaste.setToolTipText("粘贴");
      jButpaste.setHorizontalTextPosition(SwingConstants.LEFT);
      jButpaste.setMaximumSize(new Dimension(40, 37));
      jButpaste.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButpaste_actionPerformed(e);
         }
      });

      jButMerge.setIcon(imagemerge);
      jButMerge.setToolTipText("合并居中(Ctrl+D))");
      jButpaste.setHorizontalTextPosition(SwingConstants.LEFT);
      jButMerge.setMaximumSize(new Dimension(40, 37));
      jButMerge.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButMerge_actionPerformed(e);
         }
      });

      jButcopy.setIcon(imagecopy);
      jButcopy.setToolTipText("复制");
      jButcopy.setMaximumSize(new Dimension(40, 37));
      jButcopy.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButcopy_actionPerformed(e);
         }
      });
      jButcut.setIcon(imagecut);
      jButcut.setToolTipText("剪切");
      jButcut.setMaximumSize(new Dimension(40, 37));
      jButcut.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButcut_actionPerformed(e);
         }
      });
      jButPrint.setIcon(imageprint);
      jButPrint.setToolTipText("自适应打印");
      jButPrint.setMaximumSize(new Dimension(40, 37));
      jButPrint.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButPrint_actionPerformed(e);
         }
      });
      jButMod.setMaximumSize(new Dimension(40, 37));
      jButMod.setForeground(Color.magenta);
      jButMod.setEnabled(false);
      jButMod.setFont(new java.awt.Font("Dialog", 0, 12));
      jButMod.setMinimumSize(new Dimension(40, 37));
      jButMod.setPreferredSize(new Dimension(40, 37));
      jButMod.setToolTipText("修改数据");
      jButMod.setIcon(imageupdate);
    //  jButMod.setText("Mod(修改)");
      jButMod.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButMod_actionPerformed(e);
         }
      });

      jBook1.addKeyListener(new java.awt.event.KeyAdapter() {
         public void keyPressed(KeyEvent e) {
            jBook1_keyPressed(e);
         }
      });
      jMenu4.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu4.setActionCommand("行设置");
      jMenu4.setText("行设置");
      jMenu4.setIcon(rowset16);
      jMenu5.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu5.setText("列设置");
      jMenu5.setIcon(columnset16);
      jAddRow.setFont(new java.awt.Font("Dialog", 0, 12));
      jAddRow.setText("增加总行数");
      jAddRow.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jAddRow_actionPerformed(e);
         }
      });
      jDelRow.setFont(new java.awt.Font("Dialog", 0, 12));
      jDelRow.setText("减少总行数");
      jDelRow.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jDelRow_actionPerformed(e);
         }
      });

      jAddCol.setFont(new java.awt.Font("Dialog", 0, 12));
      jAddCol.setText("增加总列数");
       jAddCol.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jAddCol_actionPerformed(e);
           }
       });
       jDelCol.setFont(new java.awt.Font("Dialog", 0, 12));
       jDelCol.setText("减少总列数");
       jDelCol.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jDelCol_actionPerformed(e);
           }
       });
       jMenuevent.setFont(new java.awt.Font("Dialog", 0, 12));
       jMenuevent.setText("事项查询");
       jMenuevent.setIcon(eventquery16);
       jMenuevent.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuevent_actionPerformed(e);
         }
      });
      //jMenuItPrint.setEnabled(false);
      jMenuItPrint.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuItPrint.setText("停止定时打印");
       jMenuItPrint.setIcon(stoptimeprint16);
       jMenuItPrint.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jMenuItPrint_actionPerformed(e);
           }
       });
       jMenugrid.setFont(new java.awt.Font("Dialog", 0, 12));
       jMenugrid.setText("显示/隐藏网格");
       jMenugrid.setIcon(grid16);
       jMenugrid.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenugrid_actionPerformed(e);
         }
      });
      jMenuIRowH.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuIRowH.setText("设置行高");
      jMenuIRowH.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuIRowH_actionPerformed(e);
         }
      });
      jMenuColwidth.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuColwidth.setText("设置列宽");
      jMenuColwidth.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuColwidth_actionPerformed(e);
         }
      });
      jMenu6.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu6.setIcon(imageprintset16);
      jMenu6.setText("打印设置");
      jMenuPArea.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuPArea.setText("获得打印区域");
      jMenuPArea.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuPArea_actionPerformed(e);
         }
      });
      jMenuSArea.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuSArea.setText("设置打印区域");
      jMenuSArea.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuSArea_actionPerformed(e);
         }
      });

      jScrollTree.setHorizontalScrollBarPolicy(JScrollPane.
                                               HORIZONTAL_SCROLLBAR_ALWAYS);
      // jScrollTree.setHorizontalScrollBarPolicy(JScrollPane.
      //                                         VERTICAL_SCROLLBAR_AS_NEEDED);
      //  jScrollTree.getViewport().setBackground(Color.lightGray);
      jScrollTree.setFont(new java.awt.Font("Dialog", 0, 12));
      jScrollTree.setAutoscrolls(true);

      jMenuSetRowNums.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuSetRowNums_actionPerformed(e);
         }
      });
      jMenuSetRowNums.setText("设置总行数");
      jMenuSetRowNums.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuSetRowNums.setActionCommand("设置总行数");

      jMenuSetColNums.setText("设置总列数");
      jMenuSetColNums.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuSetColNums.setActionCommand("设置总列数");
      jMenuSetColNums.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuSetColNums_actionPerformed(e);
         }
      });
      jMenu8.setActionCommand("查询(Q)");
      jMenu3.setActionCommand("工具(T)");
      jMenu1.setActionCommand("管理(M)");
      jMenu2.setActionCommand("用户(U)");
      jScrollTree.setPreferredSize(new Dimension(200, 20));
      jSplitPane1.add(jBookpanel, JSplitPane.RIGHT);
      jBookpanel.add(jBook1,borderLayout.CENTER );
      jSplitPane1.add(jScrollTree, JSplitPane.LEFT);
      jScrollTree.setBackground(Color.white);
      jScrollTree.setBorder(new EmptyBorder(5, 5, 0, 0));


      jSplitPane1.setDividerSize(3);
 //     jScrollPane1Tree.getViewport().add(jScrollTree, null);
      jSplitPane1.setDividerLocation(200); // 设置比例
      // jScrollTree.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
      //jScrollTree.setFont(new java.awt.Font("Dialog", 0, 30));
      //jScrollTree.setAutoscrolls(true);
      //jScrollTree.setPreferredSize(new Dimension(250, 20));


      jMenutest.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenutest.setText("规律数据填充");
      jMenutest.setIcon(datafill16);
       jMenutest.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
               jMenutest_actionPerformed(e);
           }
       });
       jMenufillE.setFont(new java.awt.Font("Dialog", 0, 12));
       jMenufillE.setText("等差填充");
       jMenufillE.setIcon(equalto16);
       jMenufillE.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenufillE_actionPerformed(e);
         }
      });
      jMenuSaveAs.setText("另存为");
      jMenuSaveAs.setIcon(imagesaveas16);
      jMenuSaveAs.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
              jMenuSaveAs_actionPerformed(e);
          }
      });
      jMenu7.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu7.setText("常用公式");
      jMenu7.setIcon(formula16);
      jMenumax.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenumax.setText("最大值");
      jMenumax.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenumax_actionPerformed(e);
         }
      });
      jMenumin.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenumin.setText("最小值");
      jMenumin.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenumin_actionPerformed(e);
         }
      });
      jMenuave.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuave.setText("平均值");
      jMenuave.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuave_actionPerformed(e);
         }
      });
      jButPA.setFont(new java.awt.Font("Dialog", 0, 12));
      jButPA.setMaximumSize(new Dimension(119, 37));
      jButPA.setMinimumSize(new Dimension(79, 37));
      jButPA.setPreferredSize(new Dimension(111, 37));
      jButPA.setToolTipText("打印区域");
      jButPA.setIcon(imageprintarea);
      jButPA.setText("标识打印区域");
      jButPA.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButPA_actionPerformed(e);
         }
      });
      jComPSize.setFont(new java.awt.Font("Dialog", 0, 12));
      jComPSize.setMaximumSize(new Dimension(80, 37));
      jComPSize.setMinimumSize(new Dimension(50, 37));
      jComPSize.setPreferredSize(new Dimension(80, 37));
      jComPSize.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jComPSize_actionPerformed(e);
         }
      });
      jMenu1.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu1.setText("管理(M)");
      jMenutreeM.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenutreeM.setText("树节点管理");
       jMenutreeM.setIcon(treenode16);
      jMenutreeM.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
              jMenutreeM_actionPerformed(e);
          }
      });
      jMenu8.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenu8.setText("查询(Q)");
      jMenuSave.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuScale.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuScale.setText("设置最大显示区域");
      jMenuScale.setIcon(maxarea16);
      jMenuScale.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuScale_actionPerformed(e);
         }
      });


      jButfresh.setIcon(imagerefresh);
      jButfresh.setToolTipText("刷新");
      jButfresh.setMaximumSize(new Dimension(40, 37));

      jButfresh.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButfresh_actionPerformed(e);
         }
      });
      jBook1.setFileName("");
      jMenuabs.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuabs.setSelectedIcon(imagecheck);
      jMenuabs.setText("正值显示");
      jMenuabs.setIcon(plus16);
      jMenuabs.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuabs_actionPerformed(e);
         }
      });
      jMenuFix.setFont(new java.awt.Font("Dialog", 0, 12));
     // jMenuFix.setBorder(null);
    //  jMenuFix.setSelectedIcon(imagecheck);
      jMenuFix.setText("锁定行(列)");
      jMenuFix.setIcon(lock16);
      jMenuFix.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
              jMenuFix_actionPerformed(e);
          }
      });
      jMenumod.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenumod.setText("数据整定");
      jMenumod.setIcon(datasetting16);
      jMenumod.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
              jMenumod_actionPerformed(e);
          }
      });
      jMenudead.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenudead.setText("死数查询");
      jMenudead.setIcon(deaddata16);
      jMenudead.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
              jMenudead_actionPerformed(e);
          }
      }); //嘉善
      jMenuSheet.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuSheet.setText("插入工作表");
      jMenuSheet.setIcon(insert16);
      jMenuSheet.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuSheet_actionPerformed(e);
         }
      });

      jMenuDelSheet.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuDelSheet.setText("删除当前工作表");
      jMenuDelSheet.setIcon(deletethistable16);
      jMenuDelSheet.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuDelSheet_actionPerformed(e);
         }
      });

      jMenuChart.setFont(new java.awt.Font("Dialog", 0, 12));
      jMenuChart.setText("插入图表");
      jMenuChart.setIcon(insertchart16);
      jMenuChart.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jMenuChart_actionPerformed(e);
         }
      });

      jButFh.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButFh_actionPerformed(e);
         }
      }); ///嘉善*
      jButFh.setToolTipText("检修负荷");
      jButFh.setIcon(imageload);
      jButFh.setPreferredSize(new Dimension(40, 37));
      jButFh.setMinimumSize(new Dimension(40, 37));
      jButFh.setMaximumSize(new Dimension(40, 37));
      jButFh.setFont(new java.awt.Font("Dialog", 0, 12));
      jToolBar.add(jButnew, null);
      jToolBar.add(jButopen, null);
      jToolBar.add(jButsave, null);
      jToolBar.add(jButPrint, null);
      jToolBar.add(jButcut, null);
      jToolBar.add(jButcopy, null);
      jToolBar.add(jButpaste, null);
      jToolBar.addSeparator();
      jToolBar.add(jButMerge, null);
      jToolBar.add(jButfresh, null);
      jToolBar.addSeparator();
      jToolBar.add(jButFx, null);
      jToolBar.add(jButFk, null);
      jToolBar.add(jButSQL, null);
      jToolBar.add(jButRTDB, null);
      jToolBar.add(jButFh, null);
      jToolBar.addSeparator();
      jToolBar.add(jButFx1, null);
      jToolBar.add(jButMod, null);
      jToolBar.addSeparator();
      jToolBar.add(jButPA, null);
      jToolBar.add(jComPSize, null);
      //jToolBar.add(jButdel,null);
      // contentPane.add(jScrollTree, BorderLayout.WEST);

      contentPane.add(jSplitPane1, BorderLayout.CENTER);
//    contentPane.add(jTreetab, BorderLayout.WEST);
      //   contentPane.add(jBook1,  BorderLayout.CENTER);
//    contentPane.add(jTreetab, BorderLayout.WEST);
      jMenuFile.add(jMenuNew);
      jMenuFile.add(jMenuOpen);
      jMenuFile.add(jMenuISave);
      jMenuFile.add(jMenuSaveAs);
      jMenuFile.addSeparator();
      jMenuFile.add(jMenu6);
      jMenuFile.add(jMenuPrintPatch);
      jMenuFile.add(jMenuPrint);
      jMenuFile.add(jMenuPrintView);

     // jMenuFile.add(jMenuPrintSome);
      jMenuFile.addSeparator();
      jMenuFile.add(jMenuhtml);
      jMenuFile.add(jMenuexcel);
     // jMenuFile.add(jMenusomexls);
      jMenuFile.addSeparator();
      jMenuFile.add(jMenuFileExit);
//      jMenuHelp.add(jMenuHelpAbout);
      jMenuSave.add(jMenuFile);
      jMenuSave.add(jMenu2);
      jMenuSave.add(jMenu1);
      jMenuSave.add(jMenu8);
      jMenuSave.add(jMenu3);
//      jMenuSave.add(jMenuHelp);
      this.setJMenuBar(jMenuSave);
      contentPane.add(statusBar, BorderLayout.SOUTH);
      contentPane.add(jToolBar, BorderLayout.NORTH);
      jMenu2.add(jMenuchangeusr);
      jMenu3.add(jMenuScale);
      jMenu3.add(jMenu4);
      jMenu3.add(jMenu5);
      jMenu3.add(jMenuItPrint);
      jMenu3.add(jMenugrid);
      jMenu3.addSeparator();
      jMenu3.add(jMenufillE);
      jMenu3.add(jMenu7);
      jMenu3.add(jMenuabs);
      jMenu3.add(jMenuFix);
      jMenu3.add(jMenuSheet);
      jMenu3.add(jMenuDelSheet);
      jMenu3.add(jMenuChart);
      jMenu3.addSeparator();
      jMenu3.add(jMenutest);
      jMenu3.add(jMenumod);
      jMenu3.add(jMenudead);
      jMenu4.add(jAddRow);
      jMenu4.add(jDelRow);
      jMenu4.add(jMenuIRowH);
      jMenu4.add(jMenuSetRowNums);
      jMenu5.add(jAddCol);
      jMenu5.add(jDelCol);
      jMenu5.add(jMenuColwidth);
      jMenu5.add(jMenuSetColNums);

      jMenu6.add(jMenuPArea);
      jMenu6.add(jMenuSArea);
      jMenu7.add(jMenumax);
      jMenu7.add(jMenumin);
      jMenu7.add(jMenuave);
      jMenu1.add(jMenubbman);
      jMenu1.add(jMenutreeM);
      jMenu8.add(jMenuIpara);
      jMenu8.add(jMenuevent);
      try {
         jComPSize.addItem("A4 - 横向");
         jComPSize.addItem("A4 - 纵向");
         jComPSize.addItem("A3 - 横向");
         jComPSize.addItem("A3 - 纵向");
         /**      jComZoom.addItem("50%");
               jComZoom.addItem("75%");
               jComZoom.addItem("100%");
               jComZoom.addItem("150%");
               jComZoom.addItem("200%");
               zoom_first = false;
               jComZoom.setSelectedIndex(2);*/
         jComPSize.setSelectedIndex(0);
         jBook1.setAutoRecalc(true);
         jBook1.setNumSheets(1);
//      jBook1.setMaxCol(18);
//      jBook1.setMaxRow(40);
         bZoom = true;
//      System.out.println(jBook1.getDefaultColWidth());
//      System.out.println(jBook1.getDefaultRowHeight());
         jBook1.setShowVScrollBar(jBook1.eShowOn);
         jBook1.setShowHScrollBar(jBook1.eShowOn);
//      jBook1.setText(0,0,"fdsfsdf");
//      System.out.println(Integer.toString(jBook1.getRowHeight(0)) + ":" + Integer.toString(jBook1.getColWidth(0)));
         jBook1.setColWidth(0, jBook1.getMaxCol(),
                            jBook1.getColWidth(0) * g_gridrateu / g_gridrated, false);
         jBook1.setRowHeight(0, jBook1.getMaxRow(),
                             jBook1.getRowHeight(0) * g_gridrateu / g_gridrated, false, false);

//      System.out.println(Integer.toString(jBook1.getRowHeight(0)) + ":" + Integer.toString(jBook1.getColWidth(0)));
         int ret = f1jdll.startdb();
         if (ret < 0) {
            System.out.println("open database失败!");
            System.exit(1);
         }
         else {
//        System.out.println("open database成功!");
         }
         byte[] sacenv = f1jdll.getenvvar();
         s_sacenv = new String(sacenv);
//      System.out.println(s_sacenv);

         s_seprate = System.getProperty("file.separator");

         dlgusr1 = new usacreport.dlgusr(this, "用户登陆", true);
         dlgtreem1 = new usacreport.dlgtreem(this, "树节点管理", true);
         dlgsel = new usacreport.dlgSelect(this, "时间选择", true);
         dlgmoddata1 = new usacreport.dlgModData(this, "修改数据", true);
         dlgmodp1 = new usacreport.dlgModP(this, "修改数据窗口", true);
//      System.out.println(Integer.toString(ret));
         dlgusr1.init_usrcom();
         dlgtreem1.inittree();
         if (ret == 1) {
            Dimension dlgSize = dlgusr1.getPreferredSize();
            Dimension frmSize = Toolkit.getDefaultToolkit().getScreenSize();
            Point loc = getLocation();
            dlgusr1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                                (frmSize.height - dlgSize.height) / 2 + loc.y);
            dlgusr1.setModal(true);
            dlgusr1.pack();
//        dlgusr1.init_usrcom();
            dlgusr1.show();
         }
         else if (ret == 2) {
            g_role = 0;
         }
         else if (ret == 3) {
            g_role = 1;
         }

         role_ctrl();
         dlgfill = new usacreport.dlgFill(this, "遥测填充", true);
         dlgFillK = new usacreport.dlgFillK(this, "遥信填充", true);
         dlgFillSQL = new usacreport.dlgFillSQL(this, "SQL填充", true);
         dlgfillrtdb = new usacreport.dlgfillrtdb(this, "实时数据填充", true);
         dlgPatchPrint = new usacreport.dlgPatchPrint(this, "报表批量打印", true);
         dlgshow = new usacreport.dlgfillshow(this, "单元格描述", true);
         dlgusrman = new usacreport.dlgusrmanage(this, "报表管理", true);
         dlgpara1 = new usacreport.dlgpara(this, "参数查询", true);
         dlgEvent1 = new usacreport.dlgEvent(this, "事项查询", true);
         dlgfillgl1 = new usacreport.dlgfillgl(this, "规律数据填充", true);
         dlgprintpara1 = new usacreport.dlgPringpara(this, "打印参数", true);
         dlgfillfh1 = new usacreport.dlgfillfh(this, "检修负荷", true);
         dlgdeadatal = new usacreport.dlgdeadata(this, "死数查询", true);
         dlgExport1 = new usacreport.dlgExport(this, "批量输出", true);
         dlgPrint1 = new usacreport.dlgPrint(this, "批量打印", true);

         //  ToolTipManager.sharedInstance().registerComponent(jTreetab);
         structTree();

         b_run = true;
         fillzsxxVec();
//       this.update(this.getGraphics());
      }
      catch (java.lang.UnsatisfiedLinkError eee) {
         System.out.println(eee.getMessage());
      }
   }

   //File | Exit action performed
   public void jMenuFileExit_actionPerformed(ActionEvent e) {
      try {
         f1jdll.closedb();
      }
      catch (java.lang.UnsatisfiedLinkError ee) {
         System.out.println(ee.getMessage());
      }

      System.exit(0);
   }

   //Help | About action performed
   public void jMenuHelpAbout_actionPerformed(ActionEvent e) {
      FrmSacReport_AboutBox dlg = new FrmSacReport_AboutBox(this);
      Dimension dlgSize = dlg.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlg.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                      (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlg.setModal(true);
      dlg.pack();
      dlg.setVisible(true);
   }

   //Overridden so we can exit when window is closed
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
         jMenuFileExit_actionPerformed(null);
      }
   }

   void jMenuNew_actionPerformed(ActionEvent e) {
      fileNew();
   }

   void jMenuISave_actionPerformed(ActionEvent e) {
      fileSave(0);
   }

   void jMenuOpen_actionPerformed(ActionEvent e) {
      fileopen();
   }

   void jButopen_actionPerformed(ActionEvent e) {
      fileopen();
   }

   void jButsave_actionPerformed(ActionEvent e) {
      fileSave(0);
   }

   void jButFx_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgfill.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgfill.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                          (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgfill.setModal(false);
      dlgfill.pack();
      dlgfill.setVisible(true);
   }


   void jButFk_actionPerformed(ActionEvent e) {

      Dimension dlgSize = dlgFillK.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgFillK.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                           (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgFillK.setModal(false);
      dlgFillK.pack();
      dlgFillK.setVisible(true);

   }

   void jButSQL_actionPerformed(ActionEvent e) {

      Dimension dlgSize = dlgFillSQL.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgFillSQL.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                             (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgFillSQL.setModal(false);
      dlgFillSQL.pack();
      dlgFillSQL.setVisible(true);

   }

   void jButRTDB_actionPerformed(ActionEvent e) {

      Dimension dlgSize = dlgfillrtdb.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgfillrtdb.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                             (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgfillrtdb.setModal(false);
      dlgfillrtdb.pack();
      dlgfillrtdb.setVisible(true);

   }


   void jButdel_actionPerformed(ActionEvent e) {
      if (jBook1.getParent() != null) {
         this.setTitle("D6000报表工具");
         jBookpanel.remove(jBook1);
         flag = 0;
         jBookpanel.updateUI();
      }
}


   void jMenuhtml_actionPerformed(ActionEvent e) {
      htmlgen1();
   }

   void jButFx1_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgshow.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgshow.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                          (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgshow.setModal(true);
      dlgshow.pack();
      dlgshow.Freshtable();
      dlgshow.setVisible(true);
   }

   void jMenuchangeusr_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgusr1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgusr1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                          (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgusr1.setModal(true);
      dlgusr1.pack();
      dlgusr1.Setpass();
      //dlgusr1.show();
      dlgusr1.setVisible(true);
      role_ctrl();
   }

   void jMenuManage_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgusrman.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgusrman.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                            (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgusrman.setModal(true);
      dlgusrman.pack();
      dlgusrman.Freshlist();
      dlgusrman.setVisible(true);
   }

   void role_ctrl() {
      jMenu3.setEnabled(true);
      jMenuNew.setEnabled(true);
      jMenuOpen.setEnabled(true);
      jMenuISave.setEnabled(true);
      jMenuhtml.setEnabled(true);
      jMenuSaveAs.setEnabled(true);
      jMenu6.setEnabled(true);
      jMenuPrint.setEnabled(true);
      jMenuhtml.setEnabled(true);
      jMenuexcel.setEnabled(true);
      jButnew.setEnabled(true);
      jButopen.setEnabled(true);
      jButsave.setEnabled(true);
      jButFx.setEnabled(true);
      jButFx1.setEnabled(true);
      jButPA.setEnabled(true);
      jComPSize.setEnabled(true);
      jMenu1.setEnabled(true);
      jMenu8.setEnabled(true);
      switch (g_role) {
         case 0:
            break;
         case 1:
            jMenu1.setEnabled(false);
            jMenu8.setEnabled(false);
            jButPA.setEnabled(false);
            jComPSize.setEnabled(false);
            jMenuNew.setEnabled(false);
            jMenuOpen.setEnabled(false);
            jMenuISave.setEnabled(false);
            jMenuhtml.setEnabled(false);
            jMenu3.setEnabled(false);
            jButMod.setEnabled(false);
            jMenuSaveAs.setEnabled(false);
            jMenu6.setEnabled(false);
            jMenuPrint.setEnabled(false);
            jMenuhtml.setEnabled(false);
            jMenuexcel.setEnabled(false);
            jButnew.setEnabled(false);
            jButopen.setEnabled(false);
            jButsave.setEnabled(false);
            jButFx.setEnabled(false);
            jButFx1.setEnabled(false);
            break;
      }
   }

   void xlsgen() {
      JFileChooser chooser = new JFileChooser();
      ExampleFileFilter filter = new ExampleFileFilter();
      filter.addExtension("xls");
      chooser.setFileFilter(filter);
      File fFile = new File(s_sacenv + s_seprate + "reportfile");
      chooser.setCurrentDirectory(fFile);
      int retvalue = chooser.showSaveDialog(this);
      if (retvalue == JFileChooser.APPROVE_OPTION) {
         try {
            String filesp = System.getProperty("file.separator");
            String filename;
            filename = chooser.getSelectedFile().getPath();
            int pos = filename.lastIndexOf(".");
            if (pos < 0) {
               filename = filename + ".xls";
            }
            else {
               filename = filename.substring(0, pos);
               filename = filename + ".xls";
            }
            java.io.File file = new java.io.File(filename);
            FileOutputStream outFile = new FileOutputStream(file);
            usacreport.FrmSacReport.jBook1.write(outFile,
                                                 usacreport.FrmSacReport.jBook1.
                                                 eFileExcel97);
         }
         catch (com.f1j.util.F1Exception e) {
         }
         catch (java.io.IOException e1) {
         }
         catch (java.lang.NullPointerException e2) {
         }
      }
   }

   void jMenuexcel_actionPerformed(ActionEvent e) {
      xlsgen();
   }

   void jMenusomexls_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgpara1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgExport1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                             (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgExport1.setModal(true);
      dlgExport1.pack();
      dlgExport1.setVisible(true);
   }

   void jMenuprintsome_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgpara1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgPrint1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                            (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgPrint1.setModal(true);
      dlgPrint1.pack();
      dlgPrint1.setVisible(true);
   }

   void fileopen() {

      i = 1;
      jButMod.setEnabled(false);
      g_savemode = true;
//    rate_zoom=1.0;
//    jComZoom.setSelectedIndex(2);
      JFileChooser chooser = new JFileChooser();
      ExampleFileFilter filter = new ExampleFileFilter();
      filter.addExtension("vts");
      chooser.setFileFilter(filter);
      File fFile = new File(s_sacenv + s_seprate + "reportfile");
      chooser.setCurrentDirectory(fFile);
      int retvalue = chooser.showOpenDialog(this);
      if (retvalue == JFileChooser.APPROVE_OPTION) {
//      String currentdir = System.getProperty("user.dir");
//      String filesp = System.getProperty("file.separator");
//      direc = currentdir + filesp +"test.sac";
//      javax.swing.JOptionPane.showInputDialog(null,direc);
         String direc;
         direc = chooser.getSelectedFile().getPath(); //获得全路径
         g_newfilename = chooser.getSelectedFile().getName(); //获得文件名
         g_filename = direc;
         this.setTitle(direc);
         try {
            FileInputStream inputstream = new FileInputStream(direc);

            jBook1.read(inputstream);

            jBook1.setAllowEditHeaders(false);
            //jBook1.setAllowDesigner(true);
            jBook1.setAllowShortcutMenus(true);
            jBook1.setAllowResize(true); //是否能调整大小
            jBook1.setAllowFormulas(true); //是否激活格式化
            jBook1.setAllowTabs(true);
            jBook1.setShowEditBar(true);
            jBook1.setShowEditBarCellRef(true);
            usacreport.dlgSelect.veccontent.removeAllElements();
            bZoom = true;
//        jBook1.setColWidth(0,jBook1.getMaxCol(),jBook1.getColWidth(0)*g_gridrateu/g_gridrated,false);
//        jBook1.setRowHeight(0,jBook1.getMaxRow(),jBook1.getRowHeight(0)*g_gridrateu/g_gridrated,false,false);
            inputstream.close();
//        System.out.println(Integer.toString(jBook1.getColWidthTwips(0)));
//        System.out.println(Integer.toString(jBook1.getColWidthUnits()));
//        System.out.println(Integer.toString(jBook1.getRowHeight(0)) + ":" + Integer.toString(jBook1.getColWidth(0)));
            jBook1.setActiveCell(0, 0);
            //lcg 20090819修改直接使用文件中的名字
            //jBook1.setSheetName(0,g_newfilename.substring(0,g_newfilename.length()-4));
         }
         catch (com.f1j.util.F1Exception e1) {}
         catch (java.io.FileNotFoundException e2) {}
         catch (java.io.IOException e3) {}
      }
   }

   void htmlgen1() {
      com.f1j.ss.HTMLWriter htmlwriter = new com.f1j.ss.HTMLWriter();
      JFileChooser chooser = new JFileChooser();
      ExampleFileFilter filter = new ExampleFileFilter();
      filter.addExtension("html");
      chooser.setFileFilter(filter);
      File fFile = new File(s_sacenv + s_seprate + "reportfile");
      chooser.setCurrentDirectory(fFile);
      int retvalue = chooser.showSaveDialog(this);
      if (retvalue == JFileChooser.APPROVE_OPTION) {
         try {
            String filename = chooser.getSelectedFile().getPath();
            int pos = filename.lastIndexOf(".");
            if (pos < 0) {
               filename = filename + ".html";
            }
            else {
               filename = filename.substring(0, pos);
               filename = filename + ".html";
            }
//        System.out.println(filename);
            java.io.FileWriter iowriter = new java.io.FileWriter(filename);
            htmlwriter.setFlags(htmlwriter.ALL);
            jBook1.setShowGridLines(false);
//        htmlwriter.setFlags(htmlwriter.BORDER_TAG);
//        htmlwriter.setFlags(htmlwriter.COLSPAN_TAG);
//       htmlwriter.setFlags(htmlwriter.BGCOLOR_TAG);
//        htmlwriter.setFlags(htmlwriter.COLSPAN_TAG);
//        htmlwriter.setFlags(htmlwriter.COLSPAN_TAG);
            //htmlwriter.setFlags(htmlwriter.HEIGHT_TAG);
            htmlwriter.write(jBook1.getBook(), iowriter);
         }
         catch (java.io.IOException eee) {
         }
         catch (com.f1j.util.F1Exception e1) {
         }
      }
   }

   void fileSave(int savemode) {
      if (g_savemode == false) {
         return;
      }

      /*    jComZoom.setSelectedIndex(2);
          if(jComZoom.getSelectedIndex() != 2) {
            JOptionPane.showMessageDialog(this,"保存前请保持报表100%的比例");
            return;
          }*/
      JFileChooser chooser = new JFileChooser();
      int retvalue = 0;
      boolean b_temp = false;
      if (g_newfilename.compareTo("新报表") == 0) {
         ExampleFileFilter filter = new ExampleFileFilter();
         filter.addExtension("vts");
         chooser.setFileFilter(filter);
         File fFile = new File(s_sacenv + s_seprate + "reportfile");
         chooser.setCurrentDirectory(fFile);
         retvalue = chooser.showSaveDialog(this);
         b_temp = true;
      }
      else {
         retvalue = JFileChooser.APPROVE_OPTION;
      }

      if (retvalue == JFileChooser.APPROVE_OPTION) {
         try {
            String filename = "";
            String filenamelist = "";

            if (b_temp) {
               filename = chooser.getSelectedFile().getPath(); //无网络存盘时使用取得全路径
               filenamelist = chooser.getSelectedFile().getName();
               g_newfilename = filenamelist;
            }
            else {
               filename = s_sacenv + s_seprate + "reportfile" + s_seprate +
                   g_newfilename;
               filenamelist = g_newfilename;
            }

            int pos_1 = filename.lastIndexOf('.');
            if (pos_1 < 0) { /////////是否带后缀
               filename = filename + ".vts";
            }

            if (filenamelist.lastIndexOf(".") < 0) {
               filenamelist = filenamelist + ".vts";
//          usacreport.dlgusrmanage.vecfile.addElement(filenamelist);
            }

            boolean bAdd = true;
            for (int mm = 0; mm < usacreport.dlgusrmanage.vecfile.size(); mm++) {
               if (filenamelist.compareTo(usacreport.dlgusrmanage.vecfile.get(
                   mm).toString()) == 0) {
                  bAdd = false;
                  break;
               }
            }
//        if(bAdd){
//          usacreport.dlgusrmanage.jListAll.setListData(usacreport.dlgusrmanage.vecfile);
//        }

            jBook1.saveViewInfo();
            byte[] buf = jBook1.writeToBlob();
            java.io.File file = new java.io.File(filename);

            int i_fileexist = 0;
            if (file.exists()) {
               i_fileexist = javax.swing.JOptionPane.showConfirmDialog(this,
                   "文件存在是否覆盖!");
               //0, 是
            }
            else {
               usacreport.dlgusrmanage.vecfile.addElement(filenamelist);
            }

            if (i_fileexist == 0) {
               FileOutputStream outFile = new FileOutputStream(file);
               PreProcessor.WriteOptLogFile("修改报表文件[" + filename +"]");
               outFile.write(buf);
               outFile.flush();
               outFile.close();
               //下面一段是网络存盘代码
               int ret;
               ret = javax.swing.JOptionPane.showConfirmDialog(this, "是否网络存盘!"); //0:是  1:否  2:取消
               if (ret == 0) { //是
                  filenamelist = "reportfile" + s_seprate + filenamelist;
                  ret = f1jdll.NetFileSave(filenamelist.getBytes(), buf,
                                           buf.length, 0);
                  if (ret > 0) {
                     JOptionPane.showMessageDialog(this, "网络存盘成功!");
                  }
                  else {
                     JOptionPane.showMessageDialog(this, "网络存盘失败!");
                  }
               }
            }
         }
         catch (java.io.IOException e1) {}
         catch (com.f1j.util.F1Exception e2) {}
         catch (java.lang.NullPointerException e3) {}
      }
   }

   void jButnew_actionPerformed(ActionEvent e) {
      fileNew();
   }

   void jMenuIpara_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgpara1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgpara1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                           (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgpara1.setModal(true);
      dlgpara1.pack();
      dlgpara1.show();
   }

   void jMenuPrintView_actionPerformed(ActionEvent e) {
      Print t =new Print(this, "打印预览界面", true);
      t.setVisible(true);
//       t.setModal(true);
//       t.setLocationRelativeTo(null);
//       t.show();
//       t.pack();

      }


   void jMenuPrint_actionPerformed(ActionEvent e) {
      try {
         /*      jBook1.setPrintVCenter(true);//报表Y轴居中
               jBook1.setPrintHCenter(true);//报表X轴居中
//      jBook1.setPrintArea("$A$1:"+jBook1.formatRCNr(jBook1.getLastRow(),jBook1.getLastCol(),true));
               jBook1.setPrintScaleFitToPage(true);
               jBook1.setPrintHeader("");
               jBook1.setPrintFooter("");
               jBook1.setPrintLandscape(true);
               jBook1.setPrintGridLines(false);
               jBook1.setPrintScale(400,true,1,1);//400 fit to hole page*/
         jBook1.filePrint(true);
    //   jBook1.filePrintPreview();

      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }


   void jMenuPrintPatch_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgPatchPrint.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgPatchPrint.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                                (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgPatchPrint.setModal(false);
      dlgPatchPrint.pack();
      dlgPatchPrint.show();
   }

   void jButpaste_actionPerformed(ActionEvent e) {
      try {
         jBook1.editPaste();
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jButcopy_actionPerformed(ActionEvent e) {
      try {
         jBook1.editCopy();
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jButcut_actionPerformed(ActionEvent e) {
      try {
         jBook1.editCut();
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jButPrintview_actionPerformed(ActionEvent e) {
      try {
         jBook1.setPrintVCenter(true); //报表Y轴居中
         jBook1.setPrintHCenter(true); //报表X轴居中
         jBook1.setPrintScaleFitToPage(true);
         jBook1.setPrintHeader("");
         jBook1.setPrintFooter("");
         jBook1.setPrintScale(400, true, 1, 1); //400 fit to hole page
//      jBook1.filePrintPreview();
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jButPrint_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgprintpara1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgprintpara1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                                (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgprintpara1.setModal(true);
      dlgprintpara1.pack();
      dlgprintpara1.setVisible(true);
   }

   void jButMod_actionPerformed(ActionEvent e) {
      int startrow, startcol, endrow, endcol;
      int i, j, k;
      String dbname = "modeldb";
      String tablename = "tab";
      String fieldname = "TNAME";
      String where = "", order = "";
      int[] icount = new int[1];
      Vector vectorname = new Vector();
      String tempname = "";
      boolean bfind = false;

//////////////////////////////////////////////////////////////////////////////////////////
/////update的步骤:1,opentableinfo;2,updatearecoder(可以进行多个update);3,closetableinfo/////
//////////////////////////////////////////////////////////////////////////////////////////
      usacreport.cellpara celltemp = null;
      try {
         startrow = usacreport.FrmSacReport.jBook1.getSelStartRow();
         startcol = usacreport.FrmSacReport.jBook1.getSelStartCol();
         endrow = usacreport.FrmSacReport.jBook1.getSelEndRow();
         endcol = usacreport.FrmSacReport.jBook1.getSelEndCol();
         for (i = startrow; i <= endrow; i++) {
            for (j = startcol; j <= endcol; j++) {
               for (k = 0; k < vectorpara.size(); k++) {
                  celltemp = (usacreport.cellpara) vectorpara.get(k);
//            System.out.println(celltemp.name);
                  if (celltemp.row == i && celltemp.col == j) {
                     //查到本单元格的格式描述
                     System.out.println("Find要修改的数据!");
                     jBook1.setActiveCell(i, j);
                     String strvalue = jBook1.getCellText();
                     tablename = celltemp.databasename.toUpperCase();
                     where = "TNAME = '" + tablename + "'";
                     int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(
                         dbname.getBytes(), "tab".getBytes(),
                         fieldname.getBytes(), where.getBytes(),
                         order.getBytes(), icount, true);

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////dh,dd数据///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
                     System.out.println(celltemp.style);
                     if (celltemp.style.compareTo("dh") == 0 ||
                         celltemp.style.compareTo("dd") == 0) {
                        byte[][] buffer = new byte[5][0];
                        int[] acount = new int[5]; //name,sdate,time,data
                        acount[0] = 0;
                        acount[1] = 1;
                        acount[2] = 2;
                        acount[3] = 3;
                        acount[4] = 4;

                        for (int jj = 0; jj < vectorname.size(); jj++) {
                           if (celltemp.name.compareTo( (String) vectorname.get(
                               jj)) == 0) {
                              bfind = true;
                              break;
                           }
                        }
                        if (!bfind) {
                           vectorname.addElement(celltemp.name);
                        }

                        buffer[0] = celltemp.name.getBytes();
                        buffer[1] = Integer.toString(celltemp.sdate).getBytes();
                        buffer[2] = Integer.toString(celltemp.time).getBytes();
                        buffer[3] = "12".getBytes();
                        buffer[4] = strvalue.getBytes();
//                System.out.println(celltemp.name);
//                System.out.println(Integer.toString(celltemp.sdate));
//                System.out.println(Integer.toString(celltemp.time));
//                System.out.println(Integer.toString(celltemp.flag));
//                System.out.println(dbname+":::"+tablename);
                        JOptionPane.showMessageDialog(this,
                            celltemp.name + ":" + celltemp.sdate + ":" +
                            celltemp.time + ":" + celltemp.flag);
                        returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(
                            dbname.getBytes(), tablename.getBytes());
                        System.out.println(where + "1111");
                        if (returnv <= 0) {
                           continue;
                        } //表不存在
                        System.out.println(where + "2222");

                        returnv = usacreport.FrmSacReport.f1jdll.updateARecord(
                            buffer, acount, 3);
                        System.out.println(Integer.toString(returnv));
                        if (returnv <= 0) {
                           System.out.println("记录不存在,insert!");
                           byte[][] bufferb = new byte[6][0];
                           bufferb[0] = celltemp.name.getBytes();
                           bufferb[1] = Integer.toString(celltemp.sdate).
                               getBytes();
                           bufferb[2] = Integer.toString(celltemp.time).
                               getBytes();
                           bufferb[3] = "13".getBytes();
                           bufferb[4] = strvalue.getBytes();
                           bufferb[5] = "1".getBytes();
                           int retinsert = usacreport.FrmSacReport.f1jdll.
                               insertARecord(bufferb);

                        }
                        else {
//                  System.out.println(Integer.toString(returnv));
                           //                 System.out.println("update 日规律成功!");
                        }
                        usacreport.FrmSacReport.f1jdll.closeTableInfo();
                        continue;
                     }

                     ////////////////////////////////////////////
                     //////////////////运行存储过程///////////////
                     ////////////////////////////////////////////

//              continue;
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////e*数据///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//是否考虑不修改统计数据
                     /**              if(celltemp.style.substring(0,1).compareTo("e")==0){
                                     byte [][] buffer = new byte[4][0];
                      int []acount = new int[4];//name,flag,odate,data(otime)
                                     if(celltemp.style.compareTo("e2")==0||celltemp.style.compareTo("e4")==0||celltemp.style.compareTo("en")==0||celltemp.style.compareTo("eo")==0){
                                       //时间
                                       int pos = strvalue.indexOf(":");
                      int hour = Integer.parseInt(strvalue.substring(0,pos))*60;
                      int min = Integer.parseInt(strvalue.substring(pos+1,strvalue.length()));
                                       hour = hour * 60 + min;
                      buffer[3] = Integer.toString(hour).getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 7;
                                     }else{
                                       //数据
                                       buffer[3] = strvalue.getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 4;
                                     }

                                     buffer[0] = celltemp.name.getBytes();
                      buffer[1] = Integer.toString(celltemp.flag).getBytes();
                      buffer[2] = Integer.toString(celltemp.sdate).getBytes();

                                     System.out.println(celltemp.name);
                      System.out.println(Integer.toString(celltemp.sdate));
                      System.out.println(Integer.toString(celltemp.time));
                      System.out.println(Integer.toString(celltemp.flag));
                                     returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
                                     if(returnv <= 0){continue;}//表不存在
                      returnv = usacreport.FrmSacReport.f1jdll.updateARecord(buffer,acount,3);
                                     if(returnv <= 0){
                                       System.out.println("update 日统计失败!");
                                       byte [][] bufferb = new byte[9][0];
                                       bufferb[0] = celltemp.name.getBytes();
                      bufferb[1] = Integer.toString(celltemp.sdate).getBytes();
                      bufferb[2] = Integer.toString(celltemp.time).getBytes();
                                       bufferb[3] = "1".getBytes();
                                       bufferb[4] = strvalue.getBytes();
                                       bufferb[5] = "3".getBytes();
                      int retinsert = usacreport.FrmSacReport.f1jdll.insertARecord(bufferb);
                                       if(retinsert > 0){
                                         System.out.println("插入记录成功!");
                                       }else{
                                         System.out.println("插入记录失败!");
                                       }
                                     }else{
                                       System.out.println("update 日统计成功!");
                                     }
                      usacreport.FrmSacReport.f1jdll.closeTableInfo();
                                     continue;
                                   }
                      */
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////m*数据///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

                     /**              if(celltemp.style.substring(0,1).compareTo("m")==0){
                                     byte [][] buffer = new byte[4][0];
                      int []acount = new int[4];//name,flag,odate,data(otime)
                                     if(celltemp.style.compareTo("m2")==0||celltemp.style.compareTo("m4")==0||celltemp.style.compareTo("mn")==0||celltemp.style.compareTo("mo")==0){
                                       //时间
                                       int pos = strvalue.indexOf(":");
                      int hour = Integer.parseInt(strvalue.substring(0,pos))*60;
                      int min = Integer.parseInt(strvalue.substring(pos+1,strvalue.length()));
                                       hour = hour * 60 + min;
                      buffer[3] = Integer.toString(hour).getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 7;
                                     }else{
                                       //数据
                                       buffer[3] = strvalue.getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 4;
                                     }

                                     buffer[0] = celltemp.name.getBytes();
                      buffer[1] = Integer.toString(celltemp.flag).getBytes();
                      buffer[2] = Integer.toString(celltemp.sdate).getBytes();

                                     System.out.println(celltemp.name);
                      System.out.println(Integer.toString(celltemp.sdate));
                      System.out.println(Integer.toString(celltemp.time));
                      System.out.println(Integer.toString(celltemp.flag));
                                     returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
                                     if(returnv <= 0){continue;}//表不存在
                      returnv = usacreport.FrmSacReport.f1jdll.updateARecord(buffer,acount,3);
                                     if(returnv <= 0){
                                       System.out.println("update 月规律失败!");
                                     }else{
                                       System.out.println("update 月规律成功!");
                                     }
                      usacreport.FrmSacReport.f1jdll.closeTableInfo();
                                     continue;
                                   }
                      */
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////n*数据///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

                     /**              if(celltemp.style.substring(0,1).compareTo("n")==0){
                                     byte [][] buffer = new byte[4][0];
                      int []acount = new int[4];//name,flag,odate,data(otime)
                                     if(celltemp.style.compareTo("n2")==0||celltemp.style.compareTo("n4")==0||celltemp.style.compareTo("nn")==0||celltemp.style.compareTo("no")==0){
                                       //时间
                                       int pos = strvalue.indexOf(":");
                      int hour = Integer.parseInt(strvalue.substring(0,pos))*60;
                      int min = Integer.parseInt(strvalue.substring(pos+1,strvalue.length()));
                                       hour = hour * 60 + min;
                      buffer[3] = Integer.toString(hour).getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 7;
                                     }else{
                                       //数据
                                       buffer[3] = strvalue.getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 4;
                                     }

                                     buffer[0] = celltemp.name.getBytes();
                      buffer[1] = Integer.toString(celltemp.flag).getBytes();
                      buffer[2] = Integer.toString(celltemp.sdate).getBytes();

                                     System.out.println(celltemp.name);
                      System.out.println(Integer.toString(celltemp.sdate));
                      System.out.println(Integer.toString(celltemp.time));
                      System.out.println(Integer.toString(celltemp.flag));
                                     returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
                                     if(returnv <= 0){continue;}//表不存在
                      returnv = usacreport.FrmSacReport.f1jdll.updateARecord(buffer,acount,3);
                                     if(returnv <= 0){
                                       System.out.println("update 月统计失败!");
                                     }else{
                                       System.out.println("update 月统计成功!");
                                     }
                      usacreport.FrmSacReport.f1jdll.closeTableInfo();
                                     continue;
                                   }
                      */
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////y*数据///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

                     /**              if(celltemp.style.substring(0,1).compareTo("y")==0){
                                     byte [][] buffer = new byte[4][0];
                      int []acount = new int[4];//name,flag,odate,data(otime)
                                     if(celltemp.style.compareTo("y2")==0||celltemp.style.compareTo("y4")==0||celltemp.style.compareTo("yn")==0||celltemp.style.compareTo("yo")==0){
                                       //时间
                                       int pos = strvalue.indexOf(":");
                      int hour = Integer.parseInt(strvalue.substring(0,pos))*60;
                      int min = Integer.parseInt(strvalue.substring(pos+1,strvalue.length()));
                                       hour = hour * 60 + min;
                      buffer[3] = Integer.toString(hour).getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 7;
                                     }else{
                                       //数据
                                       buffer[3] = strvalue.getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 4;
                                     }

                                     buffer[0] = celltemp.name.getBytes();
                      buffer[1] = Integer.toString(celltemp.flag).getBytes();
                      buffer[2] = Integer.toString(celltemp.sdate).getBytes();

                                     System.out.println(celltemp.name);
                      System.out.println(Integer.toString(celltemp.sdate));
                      System.out.println(Integer.toString(celltemp.time));
                      System.out.println(Integer.toString(celltemp.flag));
                                     returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
                                     if(returnv <= 0){continue;}//表不存在
                      returnv = usacreport.FrmSacReport.f1jdll.updateARecord(buffer,acount,3);
                                     if(returnv <= 0){
                                       System.out.println("update 年规律失败!");
                                     }else{
                                       System.out.println("update 年规律成功!");
                                     }
                      usacreport.FrmSacReport.f1jdll.closeTableInfo();
                                     continue;
                                   }
                      */
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////z*数据///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

                     /**              if(celltemp.style.substring(0,1).compareTo("z")==0){
                                     byte [][] buffer = new byte[4][0];
                      int []acount = new int[4];//name,flag,odate,data(otime)

                                     if(celltemp.style.compareTo("z2")==0||celltemp.style.compareTo("z4")==0||celltemp.style.compareTo("zn")==0||celltemp.style.compareTo("zo")==0){
                                       //时间
                                       int pos = strvalue.indexOf(":");
                      int hour = Integer.parseInt(strvalue.substring(0,pos))*60;
                      int min = Integer.parseInt(strvalue.substring(pos+1,strvalue.length()));
                                       hour = hour * 60 + min;
                      buffer[3] = Integer.toString(hour).getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 7;
                                     }else{
                                       //数据
                                       buffer[3] = strvalue.getBytes();
                                       acount[0] = 0;
                                       acount[1] = 3;
                                       acount[2] = 6;
                                       acount[3] = 4;
                                     }

                                     buffer[0] = celltemp.name.getBytes();
                      buffer[1] = Integer.toString(celltemp.flag).getBytes();
                      buffer[2] = Integer.toString(celltemp.sdate).getBytes();

                                     System.out.println(celltemp.name);
                      System.out.println(Integer.toString(celltemp.sdate));
                      System.out.println(Integer.toString(celltemp.time));
                      System.out.println(Integer.toString(celltemp.flag));
                                     returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
                                     if(returnv <= 0){continue;}//表不存在
                      returnv = usacreport.FrmSacReport.f1jdll.updateARecord(buffer,acount,3);
                                     if(returnv <= 0){
                                       System.out.println("update 年统计失败!");
                                     }else{
                                       System.out.println("update 年统计成功!");
                                     }
                      usacreport.FrmSacReport.f1jdll.closeTableInfo();
                                     continue;
                                   }
                      */
                     /**
                                   System.out.println(celltemp.style);
                                   System.out.println(celltemp.databasename);
                      System.out.println(Integer.toString(celltemp.col));
                      System.out.println(Integer.toString(celltemp.row));
                      System.out.println(Integer.toString(celltemp.time));
                      System.out.println(Integer.toString(celltemp.sdate));
                      System.out.println(Integer.toString(celltemp.flag));
                      System.out.println(Integer.toString(celltemp.hour));
                      System.out.println(Integer.toString(celltemp.odate));
                      System.out.println(Integer.toString(celltemp.oflag));
                      System.out.println(Integer.toString(celltemp.otime));
                      System.out.println(Double.toString(celltemp.data));
                      System.out.println(Double.toString(celltemp.rawdata));
                                   System.out.println(celltemp.name);*/
                  }
               }
            }
         }
         String procname = g_strsdate.substring(0, 6);
         String sql = "";
         for (int jj = 0; jj < vectorname.size(); jj++) {
            tempname = (String) vectorname.get(jj);
            procname = "exec data_statics" + procname;
            // sql = procname + " (" + g_strsdate + "," + "1," + "'" + tempname +"')";
            sql = procname + " " + g_strsdate + "," + "1," + "'" + tempname +
                "'";
            int returnv = f1jdll.ExecuteCmd("modeldb".getBytes(), sql.getBytes());
            if (returnv <= 0) {
               System.out.println("执行过程失败!" );
            }
            else {
               System.out.println("执行过程成功!");
            }
         }
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jMenubbman_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgusrman.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgusrman.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                            (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgusrman.setModal(true);
      dlgusrman.pack();
      dlgusrman.freshalllist();
      dlgusrman.freshtreec();
      dlgusrman.setVisible(true);
   }

   void jBook1_keyPressed(KeyEvent e) {
      try {
         if (e.getKeyCode() != 17) {
            if (pre_key == 17) {
               if (e.getKeyCode() == java.awt.event.KeyEvent.VK_B) {
                  pre_key = 0;
                  Dimension dlgSize = dlgshow.getPreferredSize();
                  Dimension frmSize = getSize();
                  Point loc = getLocation();
                  dlgshow.setLocation( (frmSize.width - dlgSize.width) / 2 +
                                      loc.x,
                                      (frmSize.height - dlgSize.height) / 2 +
                                      loc.y);
                  dlgshow.setModal(true);
                  dlgshow.pack();
                  dlgshow.Freshtable();
                  dlgshow.setVisible(true);
               }
               else if (e.getKeyCode() == java.awt.event.KeyEvent.VK_D) {
                  jButMerge_actionPerformed(null);
               }
            }
         }
         else {
            pre_key = 17;
         }
         if (e.getKeyCode() == 18) { //alt
            /*
                     Dimension dlgSize = dlgfill.getPreferredSize();
                     Dimension frmSize = getSize();
                     Point loc = getLocation();
                     dlgfill.setLocation((frmSize.width - dlgSize.width) / 2 + loc.x, (frmSize.height - dlgSize.height) / 2 + loc.y);
                     dlgfill.setModal(true);
                     dlgfill.pack();
                     dlgfill.show();
             */
         }
         else if (e.getKeyCode() == (e.CTRL_MASK | java.awt.event.KeyEvent.VK_C)) {
            jBook1.editCopy();
         }
         else if (e.getKeyCode() + 2 ==
                  (e.CTRL_MASK | java.awt.event.KeyEvent.VK_X)) {
            jBook1.editCut(); // cut是否有错误,等遇到再处理
         }
         else if (e.getKeyCode() == (e.CTRL_MASK | java.awt.event.KeyEvent.VK_V)) {
            jBook1.editPaste();
         }

      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   void jAddRow_actionPerformed(ActionEvent e) {
      try {
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "您要增加几行(默认1行)");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            str = "1";
         jBook1.setMaxRow(jBook1.getMaxRow() + Integer.parseInt(str));
         jBook1.setRowHeight(jBook1.getMaxRow() - Integer.parseInt(str),
                             jBook1.getMaxRow(),
                             jBook1.getRowHeight(jBook1.getSelStartRow()), false, false);
      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   void jDelRow_actionPerformed(ActionEvent e) {
      try {
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "您要删除几行(默认1行)");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            str = "1";
         jBook1.setMaxRow(jBook1.getMaxRow() - Integer.parseInt(str));
      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   void jMenuSetRowNums_actionPerformed(ActionEvent e) {
      try {
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "您要设置表格的最大行数是多少？（默认是100行）");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            str = "100";
         jBook1.setMaxRow(Integer.parseInt(str));
      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   public void jMenuSetColNums_actionPerformed(ActionEvent e) {
      try {
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "您要设置表格的最大列数是多少？（默认是100列");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            str = "100";
         jBook1.setMaxCol(Integer.parseInt(str));
      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   void jAddCol_actionPerformed(ActionEvent e) {
      try {
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "您要增加几列(默认1列)");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            str = "1";
         jBook1.setMaxCol(jBook1.getMaxCol() + Integer.parseInt(str));
         jBook1.setColWidth(jBook1.getMaxCol() - Integer.parseInt(str),
                            jBook1.getMaxCol(),
                            jBook1.getColWidth(jBook1.getSelStartCol()), false);
      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   void jDelCol_actionPerformed(ActionEvent e) {
      try {
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "您要删除几列(默认1列)");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            str = "1";
         jBook1.setMaxCol(jBook1.getMaxCol() - Integer.parseInt(str));
      }
      catch (com.f1j.util.F1Exception err) {
      }
   }

   void jMenuevent_actionPerformed(ActionEvent e) {

      if (usacreport.dlgEvent.threadisrun) {
         JOptionPane.showMessageDialog(this, "上一次查询还没有完成!");
      }
      else {
         Dimension dlgSize = dlgEvent1.getPreferredSize();
         Dimension frmSize = getSize();
         Point loc = getLocation();
         dlgEvent1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                               (frmSize.height - dlgSize.height) / 2 + loc.y);
         dlgEvent1.setModal(true);
         dlgEvent1.pack();
         dlgEvent1.setVisible(true);
      }
   }

   void jMenuItPrint_actionPerformed(ActionEvent e) {
      if (jMenuItPrint.getText().compareTo("停止定时打印") == 0) {
         usacreport.dlgSelect.threadstat = false;
         jMenuItPrint.setText("定时打印停止");
      }
   }

   void jMenugrid_actionPerformed(ActionEvent e) {
      if (jBook1.isShowGridLines()) {
         jBook1.setShowGridLines(false);
      }
      else {
         jBook1.setShowGridLines(true);
      }
   }

   void jMenuIRowH_actionPerformed(ActionEvent e) {
      try {
         double nRowh = (long) jBook1.getRowHeight(jBook1.getSelStartRow());
         nRowh = nRowh / 566;
         String str_temp = "";
         str_temp = Double.toString(nRowh);
         if (str_temp.length() > 6)
            str_temp.substring(0, 6);
//      System.out.println("行高-----"+Double.toString(nRowh));
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "现在行高为:" + str_temp + "厘米");
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            return;
         nRowh = Double.parseDouble(str) * 566;
         jBook1.setRowHeight(jBook1.getSelStartRow(), jBook1.getSelEndRow(),
                             (int) nRowh, false, false);
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jMenuColwidth_actionPerformed(ActionEvent e) {
      try {
         double nColw = (long) jBook1.getColWidth(jBook1.getSelStartCol());
         nColw = nColw / 1380;
         String str_temp = "";
         str_temp = Double.toString(nColw);
         if (str_temp.length() > 6)
            str_temp.substring(0, 6);
//      System.out.println("列宽-----"+Double.toString(nColw));
         String str = javax.swing.JOptionPane.showInputDialog(this,
             "现在列宽为:" + str_temp + "厘米");
//      javax.swing.JOptionPane
         if (str == null)
            return;
         if (str.compareTo("") == 0)
            return;
         nColw = Double.parseDouble(str) * 1380;
         jBook1.setColWidth(jBook1.getSelStartCol(), jBook1.getSelEndCol(),
                            (int) nColw, false);
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jMenuPArea_actionPerformed(ActionEvent e) {
      try {
         String pArea = jBook1.getPrintArea();
         javax.swing.JOptionPane.showMessageDialog(this, "现在打印区域为:" + pArea);
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jMenuSArea_actionPerformed(ActionEvent e) {
      try {
         javax.swing.JOptionPane.showMessageDialog(this,
             "设置打印区域为:" + jBook1.getSelection());
         jBook1.setPrintArea(jBook1.getSelection());
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   private void createNodes(DefaultMutableTreeNode top) {
      DefaultMutableTreeNode category = null;
      DefaultMutableTreeNode book = null;
      int i = 0, j = 0;
      int nColCnt;
      Vector vecContent = new Vector();
      String dbname = "modeldb";
      String tablename = "webRptName";
      String fieldname = "RptName";
      String where = "", order = "";
      int[] icount = new int[1];
      int returnv = 0;
      //遍历没有可管理节点的编码
      for (i = 0; i < usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.size();
           i++) {
         category = new DefaultMutableTreeNode( (String) usacreport.
                                               FrmSacReport.dlgtreem1.
                                               vec_treenodedec.get(i));
         //添加可管理的主节点,不如"日报表"之类的.
         top.add(category);
         where = "subid = " +
             (String) usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.get(i);

         //获得该节点下的所有报表.准备插入.
         nColCnt = DBEx.SqlReadRecorders(dbname,
             "Select RptName,groupname from webRptName where " + where +
                                         " order by groupname", vecContent);
         PreProcessor.WriteLogFile("sql=" +
             "Select RptName,groupname from webRptName where " + where +
                                   " order by groupname" + "  count=" +
                                   String.valueOf(vecContent.size()) +
                                   "nColCnt="
                                   + String.valueOf(nColCnt));
         if (nColCnt > 0) {
            String strGroup = "";
            String strRptName = "";
            DefaultMutableTreeNode groupNode = null;

            for (j = 0; j < vecContent.size(); j += 2) {
               strRptName = vecContent.get(j).toString().trim();
               strGroup = vecContent.get(j + 1).toString().trim();
               if (strGroup.equalsIgnoreCase("")) { //现在对于没有组节点的直接放到管理节点下
                  book = new DefaultMutableTreeNode(strRptName);
                  category.add(book);
               }

               else if ( (groupNode == null ||
                          !groupNode.getUserObject().toString().trim().
                          equalsIgnoreCase(strGroup))
                        && !strGroup.equalsIgnoreCase("")) { //增加一个组节点
                  groupNode = new DefaultMutableTreeNode(strGroup);
                  category.add(groupNode);
                  book = new DefaultMutableTreeNode(strRptName);
                  groupNode.add(book);
               }
               else {
                  book = new DefaultMutableTreeNode(strRptName);
                  groupNode.add(book);
               }

            }
         }
         /*
                   returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                       order.getBytes(),icount,true);

                   if(returnv > 0){
                     byte [][] buffer = new byte[0][0];
          buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
          int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
                     for(j=0;j<m_ret;j++){
                       String strtemp = new String(buffer[j]);
                       book = new DefaultMutableTreeNode(strtemp.trim());
                       category.add(book);
                     }
                   }
          */
      }
   }

   public void valueChanged(TreeSelectionEvent e) {
      String stlkjl = "fdsfsfdsf";
   }

   void jTreetab_mouseClicked(MouseEvent e) {
      String dsfskjd = "";
   }

   void jMenutest_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgfillgl1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgfillgl1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                             (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgfillgl1.setModal(true);
      dlgfillgl1.pack();
      dlgfillgl1.vecselgl.removeAllElements();
      dlgfillgl1.setVisible(true);
   }

//    byte []strttt = usacreport.FrmSacReport.f1jdll.RString();
//    String str = new String(strttt);
//    System.out.println(str.trim());
   //    usacreport.FrmSacReport.f1jdll.WString("end".getBytes());
//    System.out.println("end writed!");
//    this.update(this.getGraphics());

   void jMenufillE_actionPerformed(ActionEvent e) {
      try {
         int srow = usacreport.FrmSacReport.jBook1.getSelStartRow();
         int scol = usacreport.FrmSacReport.jBook1.getSelStartCol();
         int erow = usacreport.FrmSacReport.jBook1.getSelEndRow();
         int ecol = usacreport.FrmSacReport.jBook1.getSelEndCol();
         if (srow == erow) { //列等差填充
//        usacreport.FrmSacReport.jBook1.setActiveCell(srow,scol);
            String str_1 = usacreport.FrmSacReport.jBook1.getText(srow, scol);
            if (str_1.compareTo("") == 0)
               return;
//        usacreport.FrmSacReport.jBook1.setActiveCell(srow,scol+1);
            String str_2 = usacreport.FrmSacReport.jBook1.getText(srow,
                scol + 1);
            if (str_2.compareTo("") == 0)
               return;
            int num_1 = Integer.parseInt(str_2) - Integer.parseInt(str_1);
            int num_2 = num_1;
            for (int i = scol + 1; i <= ecol; i++) {
//          usacreport.FrmSacReport.jBook1.setActiveCell(srow,i);
               usacreport.FrmSacReport.jBook1.setNumber(srow, i,
                   Integer.parseInt(str_1) + num_2);
               num_2 = num_1 + num_2;
            }
         }
         if (scol == ecol) { //行等差填充
//        usacreport.FrmSacReport.jBook1.setActiveCell(srow,scol);
            String str_1 = usacreport.FrmSacReport.jBook1.getText(srow, scol);
//        usacreport.FrmSacReport.jBook1.setActiveCell(srow+1,scol);
            String str_2 = usacreport.FrmSacReport.jBook1.getText(srow + 1,
                scol);
            int num_1 = Integer.parseInt(str_2) - Integer.parseInt(str_1);
            int num_2 = num_1;
            for (int i = srow + 1; i <= erow; i++) {
//          usacreport.FrmSacReport.jBook1.setActiveCell(i,scol);
               usacreport.FrmSacReport.jBook1.setNumber(i, scol,
                   Integer.parseInt(str_1) + num_2);
               num_2 = num_1 + num_2;
            }
         }
         usacreport.FrmSacReport.jBook1.setSelStartCol(0);
         usacreport.FrmSacReport.jBook1.setSelStartRow(0);
         usacreport.FrmSacReport.jBook1.setSelEndRow(0);
         usacreport.FrmSacReport.jBook1.setSelEndCol(0);
      }
      catch (com.f1j.util.F1Exception ee) {
      }
   }

   void fileNew() {
      i = 1;
      this.setTitle("D6000报表工具");
      jButMod.setEnabled(false);
      try {
         g_newfilename = "新报表";
         bZoom = true;
         g_savemode = true;
//      jComZoom.setSelectedIndex(2);
         jBook1.initWorkbook();
//      jBook1.setMaxCol(18);
//      jBook1.setMaxRow(40);
         /**      int i=0;
               for(i=1;i<=jBook1.getLastCol();i++)
               {
                 jBook1.setColText(i,Integer.toString(i));
               }*/
         jBook1.setColWidth(0, jBook1.getMaxCol(),
                            jBook1.getColWidth(0) * g_gridrateu / g_gridrated, false);
         jBook1.setRowHeight(0, jBook1.getMaxRow(),
                             jBook1.getRowHeight(0) * g_gridrateu / g_gridrated, false, false);
//      System.out.println(Integer.toString(jBook1.getHeight()) + ":" + Integer.toString(jBook1.getWidth()));
         /**      h_row = (int)(21.0 / 0.9);
               h_col = (int)(29.7 / 3.4);
               z_row = (int)(29.7 / 0.9);
               z_col = (int)(21.0 / 3.4);
               jBook1.setColText(1,"1");
               System.out.println(jBook1.getColText(1));
               jLabPA.setText("横向:列%" + Integer.toString(h_col) + "  行%" + Integer.toString(h_row) + "    纵向:列%"
          + Integer.toString(z_col) + "  行%" + Integer.toString(z_row));*/
      }
      catch (com.f1j.util.F1Exception e1) {
         System.out.println(e1.getMessage());
      }
   }

   void jMenuSaveAs_actionPerformed(ActionEvent e) {
      g_newfilename = "新报表";
      fileSave(1);
   }

   void jMenumax_actionPerformed(ActionEvent e) {
      try {
         int i_row = 0, i_col = 0, i_row1, i_col1 = 0;
         i_row = jBook1.getSelStartRow();
         i_row1 = jBook1.getSelEndRow();
         i_col = jBook1.getSelStartCol();
         i_col1 = jBook1.getSelEndCol();
         String f_str = "MAX(" + jBook1.getSelection() + ")";
         if ( (i_row1 != i_row) && (i_col == i_col1)) {
//        jBook1.setActiveCell(i_row1+1,i_col);
            jBook1.setFormula(i_row1 + 1, i_col, f_str);
         }
         else {
//        jBook1.setActiveCell(i_row,i_col1+1);
            jBook1.setFormula(i_row, i_col1 + 1, f_str);
         }
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jMenumin_actionPerformed(ActionEvent e) {
      try {
         int i_row = 0, i_col = 0, i_row1, i_col1 = 0;
         i_row = jBook1.getSelStartRow();
         i_row1 = jBook1.getSelEndRow();
         i_col = jBook1.getSelStartCol();
         i_col1 = jBook1.getSelEndCol();
         String f_str = "MIN(" + jBook1.getSelection() + ")";
         if ( (i_row1 != i_row) && (i_col == i_col1)) {
//        jBook1.setActiveCell(i_row1+1,i_col);
            jBook1.setFormula(i_row1 + 1, i_col, f_str);
         }
         else {
//        jBook1.setActiveCell(i_row,i_col1+1);
            jBook1.setFormula(i_row, i_col1 + 1, f_str);
         }
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jMenuave_actionPerformed(ActionEvent e) {
      try {
         int i_row = 0, i_col = 0, i_row1, i_col1 = 0;
         i_row = jBook1.getSelStartRow();
         i_row1 = jBook1.getSelEndRow();
         i_col = jBook1.getSelStartCol();
         i_col1 = jBook1.getSelEndCol();
         String f_str = "AVERAGE(" + jBook1.getSelection() + ")";
         if ( (i_row1 != i_row) && (i_col == i_col1)) {
//        jBook1.setActiveCell(i_row1+1,i_col);
            jBook1.setFormula(i_row1 + 1, i_col, f_str);
         }
         else {
//        jBook1.setActiveCell(i_row,i_col1+1);
            jBook1.setFormula(i_row, i_col1 + 1, f_str);
         }
      }
      catch (com.f1j.util.F1Exception e1) {
      }
   }

   void jButPA_actionPerformed(ActionEvent e) {
      int i = 0, j = 0;
      int total_row = 0, total_col = 0;
      boolean b_thin = false;

      if (jButPA.getText().compareTo("标识打印区域") == 0) {
         jButPA.setText("恢复原始尺寸");
         try {
            h_col = jBook1.getLastCol();
            h_row = jBook1.getLastRow();
            switch (jComPSize.getSelectedIndex()) {
               case 0: { //a4横
//            for(i=0;i<jBook1.getLastCol();i++){
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getColWidth(i);
                     total_col = total_col + j;
                     if (total_col >= A4_w * 1380) {
                        h_col = i;
//                b_thin=true;
                        break;
                     }
                  }
                  h_col = h_col + 6;
                  /**            if(!b_thin){
                                double temp = A4_w * 1380 / j;
                                h_col = (int)temp;
                                b_thin=false;
                              }*/

//            for(i=0;i<jBook1.getLastRow();i++){
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getRowHeight(i);
                     total_row = total_row + j;
                     if (total_row >= A4_h * 560) {
                        h_row = i;
                        break;
                     }
                  }
                  h_row = h_row + 6;
                  /**            if(!b_thin){
                                double temp = A4_h * 560 / j;
                                h_row = (int)temp;
                                b_thin=false;
                              }*/
                  break;
               }
               case 1: { //a4纵
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getColWidth(i);
                     total_col = total_col + j;
                     if (total_col >= A4_h * 1380) {
                        h_col = i;
                        break;
                     }
                  }
                  h_col = h_col + 2;
                  /**          if(!b_thin){
                              double temp = A4_h * 1380 / j;
                              h_col = (int)temp;
                              b_thin=false;
                            }*/
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getRowHeight(i);
                     total_row = total_row + j;
                     if (total_row >= A4_w * 560) {
                        h_row = i;
                        break;
                     }
                  }
                  h_row = h_row + 4;
                  /**          if(!b_thin){
                              double temp = A4_w * 560 / j;
                              h_row = (int)temp;
                              b_thin=false;
                            }*/
                  break;
               }
               case 2: { //a3横
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getColWidth(i);
                     total_col = total_col + j;
                     if (total_col >= A3_w * 1380) {
                        h_col = i;
                        break;
                     }
                  }
                  h_col = h_col + 8;
                  /**        if(!b_thin){
                            double temp = A3_w * 1380 / j;
                            h_col = (int)temp;
                            b_thin=false;
                          }*/
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getRowHeight(i);
                     total_row = total_row + j;
                     if (total_row >= A3_h * 560) {
                        h_row = i;
                        break;
                     }
                  }
                  h_row = h_row + 6;
                  /*        if(!b_thin){
                            double temp = A3_h * 560 / j;
                            h_row = (int)temp;
                            b_thin=false;
                          }*/
                  break;
               }
               case 3: { //a3纵
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getColWidth(i);
                     total_col = total_col + j;
                     if (total_col >= A3_h * 1380) {
                        h_col = i;
                        break;
                     }
                  }
                  h_col = h_col + 3;
                  /**      if(!b_thin){
                          double temp = A3_h * 1380 / j;
                          h_col = (int)temp;
                          b_thin=false;
                        }*/
                  for (i = 0; i < 120; i++) {
                     j = jBook1.getRowHeight(i);
                     total_row = total_row + j;
                     if (total_row >= A3_w * 560) {
                        h_row = i;
                        break;
                     }
                  }
                  h_row = h_row + 6;
                  /**      if(!b_thin){
                          double temp = A3_w * 560 / j;
                          h_row = (int)temp;
                          b_thin=false;
                        }*/
                  break;
               }
            }
//      int ret=javax.swing.JOptionPane.showConfirmDialog(this,"文件存在是否覆盖!");
            jBook1.setMaxCol(h_col);
            jBook1.setMaxRow(h_row);
//        jBook1.setViewScale(1);
         }
         catch (com.f1j.util.F1Exception e1) {
         }
         return ;
      }
      else {
         try {
            jButPA.setText("标识打印区域");
            jBook1.setMaxCol(50);
            jBook1.setMaxRow(300);
         }
         catch (com.f1j.util.F1Exception eeee) {}
         return ;
      }

   }

   public void structTree() {

      DefaultMutableTreeNode top = new DefaultMutableTreeNode("报表类型");
      createNodes(top);
      //Create a tree that allows one selection at a time.
      jTreetab = new JTree(top);
      jTreetab.setRootVisible(false);
      jTreetab.setFont(new java.awt.Font("Dialog", 0, 12));
      jTreetab.setRowHeight(18);
      jTreetab.getSelectionModel().setSelectionMode
          (TreeSelectionModel.SINGLE_TREE_SELECTION);
      jTreetab.setAlignmentX( (float) 1.0);
      jTreetab.setAlignmentY( (float) 1.0);
      jTreetab.setToggleClickCount(1); //单击展开
      jTreetab.setScrollsOnExpand(true);

      jScrollTree.getViewport().add(jTreetab, null);
      //Listen for when the selection changes.
      //        TreeSelectionListener tt = null;
      //        jTreetab.addTreeSelectionListener(tt);

      //设置主界面左边的树控件的图标
      DefaultTreeCellRenderer treeRenderer = new DefaultTreeCellRenderer();

      treeRenderer.setClosedIcon(new ImageIcon(usacreport.FrmSacReport.class.
                                               getResource("icon/nodes.png")));
      treeRenderer.setLeafIcon(new ImageIcon(usacreport.FrmSacReport.class.
                                             getResource("icon/nodes.png")));
      treeRenderer.setOpenIcon(new ImageIcon(usacreport.FrmSacReport.class.
                                             getResource("icon/parentnode.png")));
      treeRenderer.setTextSelectionColor(Color.blue);
      treeRenderer.setOpaque(true);
      treeRenderer.setBackgroundNonSelectionColor(Color.white);

      treeRenderer.setIcon(new ImageIcon(usacreport.FrmSacReport.class.
                                         getResource("icon/nodes.png")));
      /*
       treeRenderer.setLeafIcon(new ImageIcon(usacreport.FrmSacReport.class.
       getResource("icon/node.png")));
       treeRenderer.setOpenIcon(new ImageIcon(usacreport.FrmSacReport.class.
       getResource("icon/curselnode.png")));
       */
      jTreetab.setCellRenderer(treeRenderer);
      ///////////////////////



      MouseListener ml = new MouseAdapter() {
         public void mousePressed(MouseEvent e) {
            int selRow = jTreetab.getRowForLocation(e.getX(), e.getY());
            javax.swing.tree.TreePath selPath = jTreetab.getPathForLocation(e.
                getX(),
                e.getY());
            if (selRow != -1) {
               if (e.getClickCount() == 1) {
                  //                mySingleClick(selRow, selPath);
               }
               else if (e.getClickCount() == 2) {
                  javax.swing.tree.TreePath tPath = jTreetab.getSelectionPath();
                  //                System.out.println(tPath.getLastPathComponent());
                  String nodename = String.valueOf(tPath.getLastPathComponent());

                  for (int i = 0;
                       i < usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.
                       size();
                       i++) {
                     if (nodename.compareTo(usacreport.FrmSacReport.dlgtreem1.
                                            vec_treenodedec.get(i).toString()) ==
                         0) {
                        return;
                     }
                  }
                  ///hongxiang 广州测试
                  final DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                      jTreetab.
                      getLastSelectedPathComponent();
                  String nodeN;
                  if (node.getLevel() > 1 && node.isLeaf()) {
                     nodeN = node.getParent().toString();
                     if (nodeN.equalsIgnoreCase("时报表"))
                        reportflag = 1;
                     else if (nodeN.equalsIgnoreCase("周报表")) {
                        reportflag = 2;
                     }
                     else if (nodeN.equalsIgnoreCase("季报表")) {
                        reportflag = 3;
                     }
                     else
                        reportflag = 0;
                  }
                  dlgsel = new usacreport.dlgSelect(AppSacReport.frameDB,
                      "时间选择", true);


                  ///hongxiang
                  //需要判断是否是分组节点
                  DefaultMutableTreeNode tmp = (DefaultMutableTreeNode) tPath.
                      getLastPathComponent();
                  if (!tmp.isLeaf())
                     return;
                  Dimension dlgSize = dlgsel.getPreferredSize();
                  Dimension frmSize = getSize();
                  Point loc = getLocation();
                  dlgsel.setLocation( (frmSize.width - dlgSize.width) / 2 +
                                     loc.x,
                                     (frmSize.height - dlgSize.height) / 2 +
                                     loc.y);
                  dlgsel.setModal(true);
                  dlgsel.pack();
                  dlgsel.g_selfilename = nodename;

                  dlgsel.setVisible(true);

                  if (g_role == 0 && usacreport.FrmSacReport.dlgsel.bData == true) {
                     jButMod.setEnabled(true);
                  }
                  else {
                     jButMod.setEnabled(false);
                  }
               }
            }
         }
      };
      jTreetab.addMouseListener(ml);

   }

   void jMenutreeM_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgtreem1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgtreem1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                            (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgtreem1.setModal(true);
      dlgtreem1.pack();
      dlgtreem1.setVisible(true);
   }

   private JMenuItem jMenuScale = new JMenuItem();
   private JButton jButfresh = new JButton();
   private JMenuItem jMenuabs = new JMenuItem();
   private JMenuItem jMenuFix = new JMenuItem();
   private JMenuItem jMenumod = new JMenuItem();
   private JMenuItem jMenudead = new JMenuItem();
   private JMenuItem jMenuChart = new JMenuItem();
   private JMenuItem jMenuSheet = new JMenuItem();
   private JMenuItem jMenuDelSheet = new JMenuItem();

   void jMenuScale_actionPerformed(ActionEvent e) {
      try {
         int ret = JOptionPane.showConfirmDialog(this,
                                                 "您是否要把显示的最大区域设置为   " +
                                                 Integer.
                                                 toString(jBook1.getSelEndRow() +
             1) + "行----" + Integer.toString(jBook1.getSelEndCol() + 1) + "列");
         if (ret == 0) {
            jBook1.setMaxRow(jBook1.getSelEndRow());
            jBook1.setMaxCol(jBook1.getSelEndCol());
         }
      }
      catch (com.f1j.util.F1Exception e1) {
      }

   }

   void jButfresh_actionPerformed(ActionEvent e) {
      this.update(this.getGraphics());
   }

   void fillzsxxVec() {
      String dbname = "modeldb";
    String fieldname = "代码,描述";
    String tablename = "站所线系参数表";
      String where = "";
      String order = "";
      int[] icount = new int[1];
      int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.
          getBytes(), tablename.getBytes(), fieldname.getBytes(),
          where.getBytes(),
          order.getBytes(), icount, true);

      if (returnv > 0) {
         byte[][] buffer = new byte[0][0];
         buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
         int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

         for (int i = 0; i < returnv; i += 2) {
            String str1 = new String(buffer[i]);
            str1 = str1.trim();
            g_veczsxxid.addElement(str1);
            String str2 = new String(buffer[i + 1]);
            str2 = str2.trim();
            g_veczsxxdec.addElement(str2);
         }
      }
   }

   void jComPSize_actionPerformed(ActionEvent e) {
      try {
         jButPA.setText("标识打印区域");
         jBook1.setMaxCol(50);
         jBook1.setMaxRow(60);
      }
      catch (com.f1j.util.F1Exception e1) {}
   }

   void jMenuabs_actionPerformed(ActionEvent e) {
      if (jMenuabs.isSelected()) {
         jMenuabs.setSelected(false);
         jMenuabs.setIcon(null);
         babs = false;
      }
      else {
         jMenuabs.setSelected(true);
         jMenuabs.setIcon(imagecheck);
         babs = true;
      }
   }

   void jMenuFix_actionPerformed(ActionEvent e) {

      try {
         if (jMenuFix.isSelected()) {
            jMenuFix.setSelected(false);
            jMenuFix.setIcon(null);
            jBook1.setFixedCols(0);
            jBook1.setFixedRows(0);
         }
         else {
            jMenuFix.setSelected(true);
            jMenuFix.setIcon(imagecheck);
            jBook1.setFixedCol(0);
            jBook1.setFixedCols(jBook1.getSelEndCol());
            jBook1.setFixedRow(0);
            jBook1.setFixedRows(jBook1.getSelEndRow());
         }
      }
      catch (com.f1j.util.F1Exception ee) {
      }
   }

   void jMenumod_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgmoddata1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgmoddata1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                              (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgmoddata1.setModal(true);
      dlgmoddata1.pack();
      dlgmoddata1.setVisible(true);
   }

   void jMenudead_actionPerformed(ActionEvent e) {
      Dimension dlgSize = dlgdeadatal.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgdeadatal.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                              (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgdeadatal.setModal(true);
      dlgdeadatal.pack();
      dlgdeadatal.setVisible(true);
   }

   void jMenuChart_actionPerformed(ActionEvent e) {
      try {
         //jBook1.setFormula(0,0,"RAND");
         jBook1.setSelection("A1:A1");
         jBook1.editCopyDown();
         jBook1.editCopyRight();
         com.f1j.ss.GRChart chart = (com.f1j.ss.GRChart) jBook1.addObject(com.
             f1j.ss.GRObject.eChart, 1, 1, 7, 12);
         chart.setLinkRange("A1:A1", true);
         // chart.setTitle("Sample");
         chart.setChartType(com.f1j.ss.GRChart.eChart);
      }
      catch (com.f1j.util.F1Exception ee) {
      }
   }

   void jMenuSheet_actionPerformed(ActionEvent e) {
      //i++;
      try {
         if (jBook1.getSheet() > jBook1.getNumSheets() || jBook1.getSheet() < 0) {
            return;
         }
         jBook1.insertSheets(jBook1.getSheet() + 1, 1);
         //jBook1.setNumSheets(i);
         //  jBook1.setColWidth(i,jBook1.getMaxCol(),jBook1.getColWidth(i)*g_gridrateu/g_gridrated,false);
         //  jBook1.setRowHeight(i,jBook1.getMaxRow(),jBook1.getRowHeight(i)*g_gridrateu/g_gridrated,false,false);
      }
      catch (com.f1j.util.F1Exception e1) {}
   }

   void jMenuDelSheet_actionPerformed(ActionEvent e) {
      try {
         if (jBook1.getNumSheets() == 0) {
            JOptionPane.showMessageDialog(this, "只有一张工作表，不能被删除！!");
            return;
         }
         if (JOptionPane.showConfirmDialog(this, "删除工作表将不能恢复,是否删除？", "警告",
                                           JOptionPane.YES_NO_OPTION,
                                           JOptionPane.QUESTION_MESSAGE) ==
             JOptionPane.NO_OPTION) {
            return;
         }
         jBook1.deleteSheets(jBook1.getSheet(), 1);
      }
      catch (com.f1j.util.F1Exception e1) {}

   }

///嘉善*
   void jButFh_actionPerformed(ActionEvent e) {
      //dlgfillfh dialog = new dlgfillfh();
      // dialog.show();

      Dimension dlgSize = dlgfillfh1.getPreferredSize();
      Dimension frmSize = getSize();
      Point loc = getLocation();
      dlgfillfh1.setLocation( (frmSize.width - dlgSize.width) / 2 + loc.x,
                             (frmSize.height - dlgSize.height) / 2 + loc.y);
      dlgfillfh1.setModal(true);
      dlgfillfh1.pack();
      dlgfillfh1.setVisible(true);
   }

   void jButMerge_actionPerformed(ActionEvent e) {
      CellFormat obj = jBook1.getCellFormat();
      Range selRange = jBook1.getSelectedRange();

      try {
         //合并单元格
         obj.setMergeCells(true);
         /*
                 //设置边框
                 obj.setTopBorder(CellFormat.eBorderThin);
                 obj.setBottomBorder(CellFormat.eBorderThin);
                 obj.setLeftBorder(CellFormat.eBorderThin);
                 obj.setRightBorder(CellFormat.eBorderThin);
          */
         //设置对齐
         obj.setHorizontalAlignment(obj.eHorizontalAlignmentCenter);
         obj.setVerticalAlignment(obj.eVerticalAlignmentCenter);
         obj.setWordWrap(false);
         selRange.setCellFormat(obj);
      }
      catch (com.f1j.util.F1Exception ee) {
         System.out.println(ee.getMessage());
      }
   }

   /////嘉善*
   /**    try{
         int i = 0,j=0;
         if(zoom_first) return;
         int oldfontsize = jBook1.getDefaultFontSize();
         System.out.println(Integer.toString(oldfontsize));
         Font newfontttt = new Font("宋体",Font.PLAIN,500);
         jBook1.setActiveCell(5,5);
         jBook1.setFont(newfontttt);
         jBook1.setRowHeight(0,jBook1.getLastCol());
         if(1==1) return;
         switch(jComZoom.getSelectedIndex())
         {
           case 0:
           {
             for(i=0;i<100;i++){
               jBook1.setRowHeight(i,i,(int)((jBook1.getRowHeight(i)/rate_zoom)*0.5),false,false);
             }
             for(i=0;i<100;i++){
    jBook1.setColWidth(i,i,(int)((jBook1.getColWidth(i)/rate_zoom)*0.5),false);
             }

//          for(i=0;i<jBook1.getLastCol();i++){
//            for(j=0;j<jBook1.getLastRow();j++){
//              jBook1.setActiveCell(j,i);

                 int newsize =(int)( oldfontsize/rate_zoom*0.5);
//              System.out.println(Integer.toString(newsize));
                 jBook1.setDefaultFontSize(newsize);
//            }
//          }
             rate_zoom = 0.5;
             break;
           }
           case 1:
           {
             for(i=0;i<100;i++){
               jBook1.setRowHeight(i,i,(int)((jBook1.getRowHeight(i)/rate_zoom)*0.75),false,false);
             }
             for(i=0;i<100;i++){
    jBook1.setColWidth(i,i,(int)((jBook1.getColWidth(i)/rate_zoom)*0.75),false);
             }

//          for(i=0;i<jBook1.getLastCol();i++){
//            for(j=0;j<jBook1.getLastRow();j++){
//              jBook1.setActiveCell(j,i);
                 int newsize =(int)( oldfontsize/rate_zoom*0.75);
//              System.out.println(Integer.toString(newsize));
                 jBook1.setDefaultFontSize(newsize);
//            }
//          }
             rate_zoom = 0.75;
             break;
           }
           case 2:
           {
             for(i=0;i<100;i++){
               jBook1.setRowHeight(i,i,(int)((jBook1.getRowHeight(i)/rate_zoom)*1),false,false);
             }
             for(i=0;i<100;i++){
    jBook1.setColWidth(i,i,(int)((jBook1.getColWidth(i)/rate_zoom)*1),false);
             }

//          for(i=0;i<jBook1.getLastCol();i++){
//            for(j=0;j<jBook1.getLastRow();j++){
//              jBook1.setActiveCell(j,i);
                 int newsize =(int)( oldfontsize/rate_zoom*1);
//              System.out.println(Integer.toString(newsize));
                 jBook1.setDefaultFontSize(newsize);
//            }
//          }
             rate_zoom = 1;
             break;
           }
           case 3:
           {
             for(i=0;i<100;i++){
               jBook1.setRowHeight(i,i,(int)((jBook1.getRowHeight(i)/rate_zoom)*1.5),false,false);
             }
             for(i=0;i<100;i++){
    jBook1.setColWidth(i,i,(int)((jBook1.getColWidth(i)/rate_zoom)*1.5),false);
             }

//          for(i=0;i<jBook1.getLastCol();i++){
//            for(j=0;j<jBook1.getLastRow();j++){
//              jBook1.setActiveCell(j,i);
                 int newsize =(int)( oldfontsize/rate_zoom*1.5);
//              System.out.println(Integer.toString(newsize));
                 jBook1.setDefaultFontSize(newsize);
//            }
//          }
             rate_zoom = 1.5;
             break;
           }
           case 4:
           {
             for(i=0;i<100;i++){
               jBook1.setRowHeight(i,i,(int)((jBook1.getRowHeight(i)/rate_zoom)*2),false,false);
             }
             for(i=0;i<100;i++){
    jBook1.setColWidth(i,i,(int)((jBook1.getColWidth(i)/rate_zoom)*2),false);
             }

//          for(i=0;i<jBook1.getLastCol();i++){
//            for(j=0;j<jBook1.getLastRow();j++){
//              jBook1.setActiveCell(j,i);
                 int newsize =(int)( oldfontsize/rate_zoom*2);
                 jBook1.setDefaultFontSize(newsize);
//            }
//          }
             rate_zoom = 2;
             break;
           }
         }
         jBook1.setActiveCell(0,0);
       }
       catch(com.f1j.util.F1Exception e1){
     }*/
}
