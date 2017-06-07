package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.event.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * 树节点管理
 * version 1.0
 */

public class dlgtreem extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JTextField jTexttname = new JTextField();
  private JScrollPane jScrollPane1 = new JScrollPane();
  private JList jListlive = new JList();
  private JButton jButadd = new JButton();
  private JButton jButdel = new JButton();
  private JButton jButquit = new JButton();
  public static Vector vec_treenodeid = new Vector();
  public static Vector vec_treenodedec = new Vector();
  private JLabel jLabel2 = new JLabel();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JLabel jLabel5 = new JLabel();
  private JLabel jLabel6 = new JLabel();

  public dlgtreem(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgtreem() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
     panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("节点名");
    jButadd.setFont(new java.awt.Font("Dialog", 0, 12));
    jButadd.setText("增加");
    jButadd.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButadd_actionPerformed(e);
      }
    });
    jButdel.setText("删除");
    jButdel.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButdel_actionPerformed(e);
      }
    });
    jButdel.setFont(new java.awt.Font("Dialog", 0, 12));
    jButquit.setText("退出");
    jButquit.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButquit_actionPerformed(e);
      }
    });
    jButquit.setFont(new java.awt.Font("Dialog", 0, 12));
    jTexttname.setFont(new java.awt.Font("SansSerif", 0, 12));
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("存");
    jLabel3.setText("点");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("节");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel5.setText("的");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel6.setText("在");
    jLabel6.setFont(new java.awt.Font("Dialog", 0, 12));
    jScrollPane1.setFont(new java.awt.Font("Dialog", 0, 12));
    jListlive.setFont(new java.awt.Font("Dialog", 0, 12));
    this.setFont(new java.awt.Font("Dialog", 0, 12));
    panel1.add(jScrollPane1,     new XYConstraints(79, 80, 255, 253));
    jScrollPane1.getViewport().add(jListlive, null);
    panel1.add(jTexttname, new XYConstraints(79, 23, 255, 30));
    panel1.add(jLabel1, new XYConstraints(27, 25, 52, 28));
      panel1.add(jLabel4, new XYConstraints(39, 230, 20, 32));
    panel1.add(jLabel2, new XYConstraints(39, 108, 20, 32));
    panel1.add(jLabel6, new XYConstraints(39, 149, 20, 32));
    panel1.add(jLabel5, new XYConstraints(39, 190, 20, 32));
    panel1.add(jLabel3, new XYConstraints(39, 271, 20, 32));
    this.getContentPane().add(panel1, BorderLayout.WEST);
      panel1.add(jButadd, new XYConstraints(27, 358, 97, 30));
      panel1.add(jButdel, new XYConstraints(131, 358, 97, 30));
      panel1.add(jButquit, new XYConstraints(234, 358, 97, 30)); //    inittree();
  }

  void jButadd_actionPerformed(ActionEvent e) {
    if(jTexttname.getText()=="") return;
    if(jTexttname.getText()==null) return;
    if(jTexttname.getText().equals("")) return;

    String str_name = jTexttname.getText();
    String str_temp = "",webid = "";
    int n_temp = 0,n_temp1=0;

    for(int i=0;i<vec_treenodeid.size();i++){
      str_temp = (String)vec_treenodeid.get(i);
      n_temp1 = Integer.parseInt(str_temp);
      if(n_temp1 > n_temp){
        n_temp = n_temp1;
      }
    }
    if(webid.compareTo("")==0){
      webid = Integer.toString(n_temp+1);
    }
    String dbname = "modeldb";
    String tablename = "rpttree";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());

    if(returnv > 0){
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      buffer[0] = webid.getBytes();
      buffer[1] = str_name.getBytes();
      buffer[2] = "0".getBytes();
      buffer[3] = "0".getBytes();
      int m_ret = usacreport.FrmSacReport.f1jdll.insertARecord(buffer);
      usacreport.FrmSacReport.f1jdll.closeTableInfo();
      vec_treenodeid.addElement(webid);
      vec_treenodedec.addElement(str_name);
      if(m_ret > 0){
         ((usacreport.FrmSacReport)getParent()).structTree();
        //structTree();
        jListlive.setListData(vec_treenodedec);
//       System.out.println("添加成功!");
        PreProcessor.WriteOptLogFile("报表树节点管理：新增树节点[" + str_name + "]");

      }
    }
  }

  void jButdel_actionPerformed(ActionEvent e) {
    int pos = jListlive.getSelectedIndex();
    String webrpt_subid = "";
    if(pos < 0 ){
//      System.out.println("请先选中一个要删除的对象");
      return;
    }
    String str = (String)jListlive.getSelectedValue();

    int i=0;
    String dbname = "modeldb";
    String tablename = "rpttree";
    String where = "describe='" + str + "'";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());

    if(returnv > 0){
      int m_ret = usacreport.FrmSacReport.f1jdll.removeARecord(where.getBytes());
      usacreport.FrmSacReport.f1jdll.closeTableInfo();

      if(m_ret > 0){
        for(i=0;i<vec_treenodedec.size();i++){
          if(str.compareTo(vec_treenodedec.get(i).toString())==0){
            webrpt_subid = (String)vec_treenodeid.get(i);
            dbname = "modeldb";
            tablename = "webrptname";
            where = "subid=" + webrpt_subid;
            returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
            if(returnv > 0){
              m_ret = usacreport.FrmSacReport.f1jdll.removeARecord(where.getBytes());
              usacreport.FrmSacReport.f1jdll.closeTableInfo();
            }
            vec_treenodedec.remove(i);
            vec_treenodeid.remove(i);
            break;
          }
        }
        jListlive.removeAll();
        jListlive.setListData(vec_treenodedec);
        //structTree();
        ((usacreport.FrmSacReport)getParent()).structTree();

        PreProcessor.WriteOptLogFile("报表树节点管理：删除树节点[" + str + "]");
//        System.out.println("删除成功!");
      }
    }
  }

  void jButquit_actionPerformed(ActionEvent e) {
    this.hide();
  }

  void inittree(){
    String dbname = "modeldb";
    String tablename = "rpttree";
    String fieldname = "id,describe";
    String where = "1=1",order = "id";
    int []icount = new int[1];

    int i=0,j=0;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
    PreProcessor.WriteLogFile("rpttree_count="+String.valueOf(returnv/2));

    byte [][] buffer = new byte[0][0];
    if(returnv > 0){
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_1 = new String(buffer[i]);
        str_1 = str_1.trim();
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        vec_treenodeid.addElement(str_1);
        vec_treenodedec.addElement(str_2);
      }
    }
    jListlive.setListData(vec_treenodedec);
  }

  private void createNodes(DefaultMutableTreeNode top) {
        DefaultMutableTreeNode category = null;
        DefaultMutableTreeNode book = null;

        int i=0,j=0;
        String dbname = "modeldb";
        String tablename = "webRptName";
        String fieldname = "RptName";
        String where = "",order = "";
        int []icount = new int[1];
        int returnv = 0;
        for(i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.size();i++){
          category = new DefaultMutableTreeNode((String)usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i));
          top.add(category);

          where = "subid = " + (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.get(i);
          returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
              order.getBytes(),icount,true);

          if(returnv > 0){
            byte [][] buffer = new byte[0][0];
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            PreProcessor.WriteLogFile("读 webprtname" + String.valueOf(m_ret) + "  " + "where=" + where);
            for(j=0;j<m_ret;j++){
              String strtemp = new String(buffer[j]);
              book = new DefaultMutableTreeNode(strtemp.trim());
              category.add(book);
            }
          }
        }
    }
  void structTree(){
      DefaultMutableTreeNode top =
            new DefaultMutableTreeNode("ROOT");
        createNodes(top);

        //Create a tree that allows one selection at a time.
        usacreport.FrmSacReport.jTreetab = new JTree(top);
        usacreport.FrmSacReport.jTreetab.setRootVisible(false);
        usacreport.FrmSacReport.jTreetab.setFont(new java.awt.Font("Dialog", 0, 20));
        usacreport.FrmSacReport.jTreetab.setRowHeight(26);
        usacreport.FrmSacReport.jTreetab.getSelectionModel().setSelectionMode
                (TreeSelectionModel.SINGLE_TREE_SELECTION);
        usacreport.FrmSacReport.jScrollTree.getViewport().add(usacreport.FrmSacReport.jTreetab, null);
        //Listen for when the selection changes.

        MouseListener ml = new MouseAdapter() {
          public void mousePressed(MouseEvent e) {
            int selRow = usacreport.FrmSacReport.jTreetab.getRowForLocation(e.getX(), e.getY());
            javax.swing.tree.TreePath selPath = usacreport.FrmSacReport.jTreetab.getPathForLocation(e.getX(), e.getY());
            if(selRow != -1) {

              if(e.getClickCount() == 1) {
//                mySingleClick(selRow, selPath);
              }
              else if(e.getClickCount() == 2) {
                javax.swing.tree.TreePath tPath = usacreport.FrmSacReport.jTreetab.getSelectionPath();
                String nodename = String.valueOf(tPath.getLastPathComponent());
                for(int i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.size();i++){
                  if(nodename.compareTo(usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i).toString())==0){
                    return;
                  }
                }
                Dimension dlgSize = usacreport.FrmSacReport.dlgsel.getPreferredSize();
                Dimension frmSize = getSize();
                Point loc = getLocation();
                usacreport.FrmSacReport.dlgsel.setLocation((frmSize.width - dlgSize.width) / 2 + loc.x, (frmSize.height - dlgSize.height) / 2 + loc.y);
                usacreport.FrmSacReport.dlgsel.setModal(true);
                usacreport.FrmSacReport.dlgsel.pack();
                usacreport.FrmSacReport.dlgsel.g_selfilename = nodename;
                usacreport.FrmSacReport.dlgsel.show();
              }
            }
          }
        };
        usacreport.FrmSacReport.jTreetab.addMouseListener(ml);
  }
}
