package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Vector;
import  usacreport.dlgEvent.*;
import java.util.*;
import java.text.*;
import javax.swing.*;
import java.awt.*;
import javax.swing.border.*;
import java.awt.event.*;
import java.awt.Font;

/**
 * Description: 报表工具
 * Company: daqo
 * 检修负荷
 * version 1.0
 */

public class dlgfillfh extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JButton jButquit = new JButton();
  javax.swing.ButtonGroup bg = new javax.swing.ButtonGroup();
  private JComboBox jComFac = new JComboBox();
  private JLabel jLabel3 = new JLabel();
  private JScrollPane jScrollPane1 = new JScrollPane();
  private JScrollPane jScrollPane2 = new JScrollPane();
  private JList jListall = new JList();
  private JList jListsel = new JList();
  private JButton jButAdd = new JButton();
  private JButton jButDel = new JButton();
  private JButton jButOk = new JButton();
  public static Vector vecselgl = new Vector();
  private Vector vecid = new Vector();
  private Vector checkbox = new Vector();
  private ArrayList alldata = new ArrayList();
   private JLabel jLabel7 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JTextField jChanget = new JTextField();
  private JLabel jLabel8 = new JLabel();
  private JTextField jCode = new JTextField();
  private JComboBox jComyearb = new JComboBox();
  private JComboBox jCommonb = new JComboBox();
  private JComboBox jComdayb = new JComboBox();
  private JComboBox jComminb = new JComboBox();
  private JComboBox jComhourb = new JComboBox();
  private JComboBox jCommine = new JComboBox();
  private JComboBox jComhoure = new JComboBox();
  private JComboBox jComdaye = new JComboBox();
  private JComboBox jComyeare = new JComboBox();
  private JComboBox jCommone = new JComboBox();
  private dlgEvent dlg =null;


  String strr=null;
  public dlgfillfh(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgfillfh() {
    this(null, "", false);
  }


  public class CheckListItemRenderer extends JCheckBox implements ListCellRenderer
  {
    public Component getListCellRendererComponent(
        JList list,
        Object value,
        int index,
        boolean isSelected,
        boolean cellHasFocus)
    {
      CheckListItem item = (CheckListItem)value;
      this.setSelected(item.getCheck());
      this.setText(item.getText());
      this.setFont(list.getFont());
      this.setEnabled(list.isEnabled());
      return this;
    }
  }

  public class CheckListItem
  {
    boolean check;
    String text;
    int index;
    public CheckListItem(boolean check, String text,int index)
    {
      this.check = check;
      this.text = text;
      this.index = index;
    }
    public boolean getCheck() { return check; }
    public void setCheck(boolean _check) { check = _check;}
    public String getText() { return text; }
    public void setText(String _text) { text = _text; }

    public int getIndex() { return index; }
    public void setIndex(int _index) { index = _index; }
  }

  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    jButquit.setFont(new java.awt.Font("Dialog", 0, 12));
    jButquit.setActionCommand("jButquit");
    jButquit.setText("退出");
    jButquit.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButquit_actionPerformed(e);
      }
    });
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("厂站名");
    jButAdd.setFont(new java.awt.Font("Dialog", 0, 12));
    jButAdd.setText(">>");
    jButAdd.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButAdd_actionPerformed(e);
      }
    });
    jButDel.setFont(new java.awt.Font("Dialog", 0, 12));
    jButDel.setText("<<");
    jButDel.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButDel_actionPerformed(e);
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
    jButOk.setText("所选遥测相加");
    jButOk.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
    jButOk.setActionCommand("jButquit");
    jListall.setFont(new java.awt.Font("SansSerif", 0, 12));
    jComyearb.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComyearb_actionPerformed(e);
      }
    });
    jComyearb.setFont(new java.awt.Font("Dialog", 0, 12));
    jCommonb.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jCommonb_actionPerformed(e);
      }
    });
    jCommonb.setFont(new java.awt.Font("Dialog", 0, 12));
    jComdayb.setFont(new java.awt.Font("Dialog", 0, 12));
    jComminb.setFont(new java.awt.Font("Dialog", 0, 12));
    jComhourb.setFont(new java.awt.Font("Dialog", 0, 12));
    jCommine.setFont(new java.awt.Font("Dialog", 0, 12));
    jComhoure.setFont(new java.awt.Font("Dialog", 0, 12));
    jComdaye.setFont(new java.awt.Font("Dialog", 0, 12));
    jComyeare.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComyeare_actionPerformed(e);
      }
    });
    jComyeare.setFont(new java.awt.Font("Dialog", 0, 12));
    jCommone.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jCommone_actionPerformed(e);
      }
    });
    jCommone.setFont(new java.awt.Font("Dialog", 0, 12));
    panel1.setMinimumSize(new Dimension(580, 310));
    jCode.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
    jCode.setText("SsysjxloadP");
    jButRm.setActionCommand("jButquit");
    jButRm.setFont(new java.awt.Font("Dialog", 0, 12));
    jButRm.setText("删除检修计划");
    jButRm.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButRm_actionPerformed(e);
      }
    });
      jChanget.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
    panel1.add(jScrollPane1,             new XYConstraints(21, 55, 200, 172));
    jScrollPane1.getViewport().add(jListall, null);
    jListall.addMouseListener(new java.awt.event.MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        if(e.getClickCount() == 2){
          OnAdd();
        }
//        jListall_mouseClicked(e);
      }
    });
    jListsel.addMouseListener(new CheckListMouseListener());
    jListsel.setFont(new java.awt.Font("Dialog", 0, 12));
      jListsel.addMouseListener(new java.awt.event.MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        if(e.getClickCount()==2){
          OnDel();
        }
//        jListsel_mouseClicked(e);
      }
    });
    jComFac.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel7.setText("至");
    jLabel4.setText("代替时间");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel8.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel8.setText("代码");
    panel1.setPreferredSize(new Dimension(580, 310));
      panel1.add(jComyearb,  new XYConstraints(23, 239, 63, 24));
    panel1.add(jCommonb,    new XYConstraints(86, 239, 45, 24));
    panel1.add(jComdayb,     new XYConstraints(131, 239, 45, 24));
    panel1.add(jComhourb,     new XYConstraints(176, 239, 45, 24));
    panel1.add(jComminb,  new XYConstraints(221, 239, 45, 24));
    panel1.add(jLabel7,  new XYConstraints(284, 239, -1, -1));
      panel1.add(jComyeare,    new XYConstraints(315, 239, 63, 24));
    panel1.add(jCommone,  new XYConstraints(379, 239, 45, 24));
    panel1.add(jComdaye,  new XYConstraints(424, 239, 45, 24));
    panel1.add(jComhoure,  new XYConstraints(469, 239, 45, 24));
    panel1.add(jCommine,  new XYConstraints(514, 239, 45, 24));
      panel1.add(jLabel4, new XYConstraints(22, 276, 67, 27));
      panel1.add(jLabel3, new XYConstraints(21, 11, 50, 27));
      panel1.add(jButquit, new XYConstraints(471, 11, 89, 26));
      panel1.add(jButRm, new XYConstraints(454, 272, -1, -1));
      panel1.add(jButOk, new XYConstraints(334, 272, -1, -1));
      panel1.add(jLabel8, new XYConstraints(187, 272, 33, 27));
      panel1.add(jCode, new XYConstraints(227, 272, 94, 27));
      panel1.add(jChanget, new XYConstraints(81, 272, 99, 27));
      panel1.add(jButAdd, new XYConstraints(253, 81, 57, 28));
      panel1.add(jButDel, new XYConstraints(253, 165, 58, 28));
      this.getContentPane().add(panel1, BorderLayout.CENTER);//确定只能单选
      panel1.add(jScrollPane2, new XYConstraints(340, 53, 220, 175));
      panel1.add(jComFac, new XYConstraints(75, 11, 145, 24));
      jScrollPane2.getViewport().add(jListsel);
    int i;String ii=null;

      int year = Calendar.getInstance().get(Calendar.YEAR); //获取当前年份 modify by hongxiang
      for(i = 2000;i <= year;i++) {
      jComyearb.addItem(Integer.toString(i));
      jComyeare.addItem(Integer.toString(i));
    }
    for(i=1;i<13;i++)
    {
      ii=Integer.toString(i);
      if (Integer.parseInt(ii)<10)  ii="0"+ii;
      jCommonb.addItem(ii);
      jCommone.addItem(ii);
    }
    for(i=0;i<=24;i++)
    {
      ii=Integer.toString(i);
      if (Integer.parseInt(ii)<10)  ii="0"+ii;
      jComhourb.addItem(ii);
      jComhoure.addItem(ii);
    }
    for(i=0;i<60;i++)
    {
      ii=Integer.toString(i);
      if (Integer.parseInt(ii)<10)  ii="0"+ii;
      jComminb.addItem(ii);
      jCommine.addItem(ii);
    }

    dayfill(1,0);
    dayfill(1,1);
    Calendar cal1 = Calendar.getInstance();
    cal1.set(Calendar.ZONE_OFFSET, 28800000);

    Date currTime = new Date();
    SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd",Locale.CHINA);
    strr=new String(formatter.format(currTime).getBytes("iso-8859-1"));
    jChanget.setText(strr);

    try{
      jComyearb.setSelectedIndex(cal1.get(cal1.YEAR)-2000);
      jComyeare.setSelectedIndex(cal1.get(cal1.YEAR)-2000);
      jCommonb.setSelectedIndex(cal1.get(cal1.MONTH));
      jCommone.setSelectedIndex(cal1.get(cal1.MONTH));
      jComdayb.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      jComdaye.setSelectedIndex(cal1.get(cal1.DAY_OF_MONTH)-1);
      jComhoure.setSelectedIndex(cal1.get(cal1.HOUR_OF_DAY));
      }catch(java.lang.IllegalArgumentException elegal){}


    try{
     // jComdb.setSelectedIndex(0);
    }catch(java.lang.IllegalArgumentException eill){}

    try{
      //jComtype.setSelectedIndex(0);
    }catch(java.lang.IllegalArgumentException eill1){}

////////////////////////读取厂站名//////////////////////
    String dbname = "modeldb";
    String tablename = "终端参数表";
    String fieldname = "描述";
    String where = "",order = "描述";

    int []icount = new int[1];
    int j = 0;
    int returnv = FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
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
    }
  }

  void jButquit_actionPerformed(ActionEvent e) {
    this.hide();
  }

  byte [][] buffer = new byte[0][0];
  String str_1=null;
  private JButton jButRm = new JButton();
  void jComFac_actionPerformed(ActionEvent e) {
    String dbname = "modeldb";
    String tablename = "终端参数表";

    String fieldname = "终端序号";
    String where = "",order = "";

    jListall.removeAll();
    vecid.removeAllElements();
    where = "描述 = '" + jComFac.getSelectedItem() + "'";
    java.util.Vector vecall = new java.util.Vector();

    int []icount = new int[1];
    int i = 0,j = 0;
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);
    if(returnv <= 0){
      return;
    }

    buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
    int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

    str_1 = new String (buffer[0]);//终端序号
    str_1 = str_1.trim();
    //if(jComdb.getSelectedIndex() == 0){//遥测库
      tablename = "遥测参数表";
      fieldname = "代码,描述";
     /* switch(jComtype.getSelectedIndex()){
        case 0:
        {
          where = "终端序号 = " + str_1;
          break;
        }
        case 1:
        {
          where = "终端序号 = " + str_1 + " and 类型 >= 20 and 类型 <= 26";
          break;
        }
        case 2:
        {
          where = "终端序号 = " + str_1 + " and 类型 >= 40 and 类型 <= 46";
          break;
        }
        case 3:
        {
          where = "终端序号 = " + str_1 + " and 类型 >= 1 and 类型 <= 10";
          break;
        }
        case 4:
        {
          where = "终端序号 = " + str_1 + " and 类型 >= 150 and 类型 <= 159";
          break;
        }
        case 5:
        {
          where = "终端序号 = " + str_1 + " and 类型 = 51";
          break;
        }
      }*/
      where = "终端序号 = " + str_1 + " and 类型 = 20";
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
//      System.out.println(where);
      if(returnv <= 0){
        return;
      }
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_3 = new String(buffer[i]);//代码
        str_3 = str_3.trim();
        vecid.addElement(str_3);
        String str_2 = new String(buffer[i+1]);//id
        str_2 = str_2.trim();
        vecall.addElement(str_2);
      }
   /* }else{
      tablename = "电度参数表";
      fieldname = "代码,描述";
      where = "终端序号 = " + str_1;
      returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                        order.getBytes(),icount,true);
      if(returnv <= 0){
        return;
      }
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
      for(i=0;i<returnv;i+=2){
        String str_3 = new String(buffer[i]);
        str_3 = str_3.trim();
        vecid.addElement(str_3);
        String str_2 = new String(buffer[i+1]);
        str_2 = str_2.trim();
        vecall.addElement(str_2);
      }
    }*/
    jListall.setListData(vecall);
  }

  void jButAdd_actionPerformed(ActionEvent e) {
    OnAdd();
  }

  void jButDel_actionPerformed(ActionEvent e) {
    OnDel();
  }
  void jButOk_actionPerformed(ActionEvent e) {
    insertdate();
  }
  int i=0;
  void OnAdd(){
    if(jListall.getSelectedIndex() < 0) {
      return;
    }
    String yearb=(String)jComyearb.getSelectedItem()+"-";
    String monb=(String)jCommonb.getSelectedItem()+"-";
    String dayb=(String)jComdayb.getSelectedItem()+" ";
    String hourb=(String)jComhourb.getSelectedItem()+":";
    String minb=(String)jComminb.getSelectedItem();

    String yeare=(String)jComyeare.getSelectedItem()+"-";
    String mone=(String)jCommone.getSelectedItem()+"-";
    String daye=(String)jComdaye.getSelectedItem()+" ";
    String houre=(String)jComhoure.getSelectedItem()+":";
    String mine=(String)jCommine.getSelectedItem();

   final String dateb = (String)yearb+monb+dayb+hourb+minb;
   final String datee = (String)"至"+yeare+mone+daye+houre+mine;

    for(int i=0;i<vecselgl.size();i++){
      CheckListItem item = (CheckListItem)jListsel.getModel().getElementAt(i);
      if(item.getText().equals(vecid.get(jListall.getSelectedIndex())+" "+dateb+datee)){
        javax.swing.JOptionPane.showMessageDialog(this,"存在代码");
        return;
      }
    }

    jListsel.setCellRenderer(new CheckListItemRenderer());  //装我们自订的cellRenderer
    DefaultListModel listModel = new DefaultListModel();
    jListsel.setModel(listModel);

    CheckListItem  item = new CheckListItem(false,(String)vecid.get(jListall.getSelectedIndex())+" "+dateb+datee,i++);
    //listModel.addElement(item);
    vecselgl.addElement(item);
   // vecselgl.addElement((String)vecid.get(jListall.getSelectedIndex())+" "+dateb+datee);
    jListsel.setListData(vecselgl);
  }
  class CheckListMouseListener extends MouseAdapter
  {
    public void mouseClicked(MouseEvent e) {
        JList list = (JList) e.getSource();
        int index = list.locationToIndex(e.getPoint());
        CheckListItem item = (CheckListItem)list.getModel().getElementAt(index);
        item.setCheck(! item.getCheck());
        Rectangle rect = list.getCellBounds(index, index);
        list.repaint(rect);
    }
  }
  void OnDel(){
    i--;
    if(jListsel.getSelectedIndex() < 0) {
      return;
    }
    for(int i=0;i<vecselgl.size();i++){
      if(vecselgl.get(i).equals(jListsel.getSelectedValue())){
//        javax.swing.JOptionPane.showMessageDialog(this,"删除成功");
        vecselgl.remove(i);
        jListsel.setListData(vecselgl);
        return;
      }
    }
  }
  void insertdate(){
    String s1 = "1970-01-01";
    String dbname = "modeldb";
    String tablename = "";
    String fieldname = "";
    String where = "",order = "",code = "";
    int records  = 0;
    int[] icount = new int[1];
    String jct = jChanget.getText();
    String jcode = jCode.getText();

    SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd",Locale.CHINA);
  //  SimpleDateFormat formatter = new SimpleDateFormat ("yyyy-MM-dd");
    ParsePosition pos = new ParsePosition(0);
    java.util.Date dt1=formatter.parse(s1,pos);

    ParsePosition pos3 = new ParsePosition(0);
    java.util.Date dt4=formatter.parse(jct,pos3);
    long l3 = (dt4.getTime() - dt1.getTime())/(3600*24*1000);


    tablename="yc"+strr.substring(0,4)+strr.substring(5,7);
    where = "name = '"+jcode+"' and sdate="+l3+" and flag = 51";
    System.out.print(where);
    records = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
        order.getBytes(),icount,true);
//      System.out.println(where);
    if(records > 0){
      JOptionPane.showMessageDialog(null,
                                    "不能重复操作！",
                                    "警告",
                                    JOptionPane.ERROR_MESSAGE);
      return;
    }

    if (dt4==null)return;

    if(vecselgl.size()<=0) return;//没有记录被选中
     ArrayList al1= new ArrayList();
     ArrayList al2= new ArrayList();
     ArrayList al3= new ArrayList();
    int i = 0,j = 0;
    Object[] objsel = jListsel.getSelectedValues();

    String data="0";
    double data2=0;
    double last=0;
    // if(jListsel.isSelectedIndex(0)==false && jListsel.isEnabled()){
    long l1 =0;
    long l2=0;
   // for(i=0;i<objsel.length;i++){
    for(i=0;i<vecselgl.size();i++){
      ParsePosition pos1 = new ParsePosition(0);
      ParsePosition pos2 = new ParsePosition(0);

      //if(jListsel.isSelectedIndex(i)){
      code=(String)vecid.get(i);
     // String str=(String)objsel[i];
      CheckListItem item = (CheckListItem)jListsel.getModel().getElementAt(i);
      boolean flag = item.getCheck();
      String str = item.getText();
      int a=str.indexOf(" ");
      int b=str.indexOf("至");

      String name=str.substring(0,a);

      String yearb=str.substring(a+1,a+11);
      java.util.Date dt2=formatter.parse(yearb,pos1);
      l1 = (dt2.getTime() - dt1.getTime())/(3600*24*1000);

      String hourb=str.substring(a+12,a+14);
      String minb=str.substring(a+15,a+17);
      minb=String.valueOf(Integer.parseInt(hourb)*60+Integer.parseInt(minb));

      String yeare=str.substring(b+1,b+11);
      java.util.Date dt3=formatter.parse(yeare,pos2);
      l2 = (dt3.getTime() - dt1.getTime())/(3600*24*1000);
      if (l1==l2){

        String houre=str.substring(b+12,b+14);
        String mine=str.substring(b+15,b+17);
        mine=String.valueOf(Integer.parseInt(houre)*60+Integer.parseInt(mine));

      //  alldata.clear();
        tablename="yc"+jComyearb.getSelectedItem()+jCommonb.getSelectedItem();
        // where = "name = '"+name.trim()+"' and FLAG <=50 and sdate="+l1+" and time>="+minb+" and time<="+mine+" and mod(time,15)=0";
        where = "name = '"+name.trim()+"' and sdate="+l1+" and FLAG <=50 and mod(time,15)=0 and time>0";
        System.out.print(where);
        records = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
            order.getBytes(),icount,true);
//      System.out.println(where);
        if(records <= 0){
          return;
        }
        buffer = new byte[records][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
        int n=0;
        for(int ii=4;ii<records;ii+=6){
          data = new String(buffer[ii]);//值
          n++;
          if ((n>=Integer.parseInt(minb)/15)&&(n<=Integer.parseInt(mine)/15))
          {
            data = data.trim();
            data2=Math.abs(Double.parseDouble(data));
            if (flag==true){
              data2=-data2;
            }
            data=Double.toString(data2);
          }else
          {
            data="0";
          }
          alldata.add(data);
          al1.add(data);
        }
      }
    }
    /*    for(int ii=2;ii<records;ii+=6){
          String time = new String(buffer[ii]).trim();//值
          al3.add(time);
        }*/
    int n=0;
    for(i=0;i<vecselgl.size();i++){
      CheckListItem item = (CheckListItem)jListsel.getModel().getElementAt(i);
      String str = item.getText();
    //  String str=(String)objsel[i];
      int a=str.indexOf(" ");
      String name=str.substring(0,a);

      tablename="yc"+jComyearb.getSelectedItem()+jCommonb.getSelectedItem();
      where = "name = '"+name.trim()+"' and sdate="+l1+" and FLAG <=50 and mod(time,15)=0 and time>0";
      records = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
          order.getBytes(),icount,true);
      if(records <= 0){
        return;
      }
      if (l1==l2){
        double ob3=0;
        double ob1=0;
        for(int ii=i*records/6;ii<(i+1)*records/6;ii++){
          if (vecselgl.size()>=2){
            if (i>=2)
            {
          ob1= Double.parseDouble(al1.get(ii-records/6).toString());
        }
        else
        {
          ob1= Double.parseDouble(al1.get(ii%(records/6)).toString());
        }
        double ob2= Double.parseDouble(alldata.get(ii).toString());
        if (i>=1){
          ob3=ob2+ob1;
        }else{
          ob3=ob2;
        }
        al2.add(Double.toString(ob3));
          }else
          {
            ob1= Double.parseDouble(al1.get(ii).toString());
            al2.add(Double.toString(ob1));
          }
        }
      }
       al1=al2;
    }
    if (vecselgl.size()>=2){
      n=i;
    }else {
      n=1;
 }
         tablename="yc"+jct.substring(0,4)+jct.substring(5,7);
         int returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());
         int retinsert=0;
         for(int ii=(n-1)*records/6;ii<n*96;ii++){
          // for(int ii=0;ii<records/6;ii++){
           byte [][] bufferb = new byte[6][0];
           bufferb[0] = jcode.getBytes();
           bufferb[1] = Long.toString(l3).getBytes();
          //bufferb[2] = Integer.toString(ii%(records/6)*15).getBytes();
          // bufferb[2] = Integer.toString(((ii%(records/6))+1)*15).getBytes();
           //int bbb=ii%(records/6)*15;
          // bufferb[3] = "51".getBytes();
           String aaa=null;
           try{
             bufferb[2] = Integer.toString(((ii%(records/6))+1)*15).getBytes();
             bufferb[3] = "51".getBytes();
             //if (objsel.length>=2){
             bufferb[4] = al1.get(ii).toString().getBytes();
               // aaa= al2.get(ii).toString();
                //int dsf=0;
            // }else
            // {
              // bufferb[4] = al1.get(ii).toString().getBytes();
            // }
           }catch(IndexOutOfBoundsException e) {
             bufferb[2] = Integer.toString(((ii%96)+1)*15).getBytes();
             bufferb[3] = "51".getBytes();
             bufferb[4] = "0".getBytes();
           }
           bufferb[5] = "0".getBytes();
           retinsert = usacreport.FrmSacReport.f1jdll.insertARecord(bufferb);
         }
         if(retinsert > 0){
           System.out.println("插入记录成功!");
           JOptionPane.showMessageDialog(null,
                              "插入记录成功！",
                              "OK",
                              JOptionPane.INFORMATION_MESSAGE);
                 //   return;
         }else{
           System.out.println("插入记录失败!");
           JOptionPane.showMessageDialog(null,
                              "插入记录失败！",
                              "警告",
                              JOptionPane.ERROR_MESSAGE);
                    return;
         }
         usacreport.FrmSacReport.f1jdll.closeTableInfo();
         alldata.clear();
         al1.clear();
         al2.clear();


   /* if(jComdb.getSelectedIndex()==1){
      tablename = "电度参数表";
    }

   /* try{///设定需要的总行数和列数
      if(jRb1.isSelected()){
        if(jCB.isSelected()){
         if(usacreport.FrmSacReport.jBook1.getMaxCol() < col + 28){
           usacreport.FrmSacReport.jBook1.setMaxCol(col + 29);
         }
         if(usacreport.FrmSacReport.jBook1.getMaxRow() < row + vecselgl.size()){
           usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
         }
        }
        else{
          if(usacreport.FrmSacReport.jBook1.getMaxRow() < row + 28){
            usacreport.FrmSacReport.jBook1.setMaxRow(row + 29);
          }
          if(usacreport.FrmSacReport.jBook1.getMaxCol() < col + vecselgl.size()){
            usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
          }
        }
      }
      else if(jRb2.isSelected()){
        if(jCB.isSelected()){
         if(usacreport.FrmSacReport.jBook1.getMaxCol() < col + 35){
           usacreport.FrmSacReport.jBook1.setMaxCol(col + 36);
         }
         if(usacreport.FrmSacReport.jBook1.getMaxRow() < row + vecselgl.size()){
           usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
         }
        }
        else{
          if(usacreport.FrmSacReport.jBook1.getMaxRow() < row + 35){
            usacreport.FrmSacReport.jBook1.setMaxRow(row + 36);
          }
          if(usacreport.FrmSacReport.jBook1.getMaxCol() < col + vecselgl.size()){
            usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
          }
        }
      }
      else if(jRb3.isSelected()){
        if(jCB.isSelected()){
         if(usacreport.FrmSacReport.jBook1.getMaxCol() < col + 16){
           usacreport.FrmSacReport.jBook1.setMaxCol(col + 17);
         }
         if(usacreport.FrmSacReport.jBook1.getMaxRow() < row + vecselgl.size()){
           usacreport.FrmSacReport.jBook1.setMaxRow(row + vecselgl.size());
         }
        }
        else{
          if(usacreport.FrmSacReport.jBook1.getMaxRow() < row + 16){
            usacreport.FrmSacReport.jBook1.setMaxRow(row + 17);
          }
          if(usacreport.FrmSacReport.jBook1.getMaxCol() < col + vecselgl.size()){
            usacreport.FrmSacReport.jBook1.setMaxCol(col + vecselgl.size());
          }
        }
      }
      usacreport.FrmSacReport.jBook1.setRowHeight(0,usacreport.FrmSacReport.jBook1.getLastRow(),usacreport.FrmSacReport.jBook1.getRowHeight(0),false,false);
      usacreport.FrmSacReport.jBook1.setColWidth(0,usacreport.FrmSacReport.jBook1.getLastCol(),usacreport.FrmSacReport.jBook1.getColWidth(0),false);
    }catch(com.f1j.util.F1Exception e2){
    }

    if(jRb1.isSelected()){
      for(i=0;i<vecselgl.size();i++){
          String str_1 = (String)vecselgl.get(i);
          try{
            if(jCB.isSelected()){//横向
              if(i==0){
                 for(j=col;j<col+24;j++){
                     usacreport.FrmSacReport.jBook1.setText(row-1,j,Integer.toString(j-col+1));
                   }
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+24,"最大值");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+25,"发生时间");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+26,"最小值");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+27,"发生时间");
                 }
                 usacreport.FrmSacReport.jBook1.setText(row+i,col-1,(String)vecselgl.get(i));//描述
                 code = "@dh," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col,code);
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+1,code);
                 code = "@es," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+24,code);
                 code = "@et," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+25,code);
                 code = "@eu," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+26,code);
                 code = "@ev," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+27,code);
            }
            else{
              if(i==0){
                for(j=row;j<row+24;j++){
                    usacreport.FrmSacReport.jBook1.setText(j,col-1,Integer.toString(j-row+1)+":00");
                  }
                  usacreport.FrmSacReport.jBook1.setText(row+24,col-1,"最大值");
                  usacreport.FrmSacReport.jBook1.setText(row+25,col-1,"发生时间");
                  usacreport.FrmSacReport.jBook1.setText(row+26,col-1,"最小值");
                  usacreport.FrmSacReport.jBook1.setText(row+27,col-1,"发生时间");
                }
                usacreport.FrmSacReport.jBook1.setText(row-1,col+i,(String)vecselgl.get(i));//描述
                code = "@dh," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row,col+i,code);
                usacreport.FrmSacReport.jBook1.setText(row+1,col+i,code);
                code = "@es," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+24,col+i,code);
                code = "@et," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+25,col+i,code);
                code = "@eu," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+26,col+i,code);
                code = "@ev," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+27,col+i,code);
              }
          }catch(com.f1j.util.F1Exception e1){

          }
        }
//      }
    }else if(jRb2.isSelected()){
      for(i=0;i<vecselgl.size();i++){
        String str_1 = (String)vecselgl.get(i);
          try{
            if(jCB.isSelected()){//横向
              if(i==0){
                 for(j=col;j<col+31;j++){
                     usacreport.FrmSacReport.jBook1.setText(row-1,j,Integer.toString(j-col+1)+"号");
                   }
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+31,"最大值");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+32,"发生时间");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+33,"最小值");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+34,"发生时间");
                 }
                 usacreport.FrmSacReport.jBook1.setText(row+i,col-1,(String)vecselgl.get(i));//描述
                 code = "@ms," + str_1 + ",0";;
                 usacreport.FrmSacReport.jBook1.setText(row+i,col,code);
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+1,code);
                 code = "@ns," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+31,code);
                 code = "@nt," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+32,code);
                 code = "@nu," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+33,code);
                 code = "@nv," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+34,code);
            }
            else{
              if(i==0){
                for(j=row;j<row+31;j++){
                    usacreport.FrmSacReport.jBook1.setText(j,col-1,Integer.toString(j-row+1));
                  }
                  usacreport.FrmSacReport.jBook1.setText(row+31,col-1,"最大值");
                  usacreport.FrmSacReport.jBook1.setText(row+32,col-1,"发生时间");
                  usacreport.FrmSacReport.jBook1.setText(row+33,col-1,"最小值");
                  usacreport.FrmSacReport.jBook1.setText(row+34,col-1,"发生时间");
                }
                usacreport.FrmSacReport.jBook1.setText(row-1,col+i,(String)vecselgl.get(i));//描述
                code = "@ms," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row,col+i,code);
                usacreport.FrmSacReport.jBook1.setText(row+1,col+i,code);
                code = "@ns," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+31,col+i,code);
                code = "@nt," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+32,col+i,code);
                code = "@nu," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+33,col+i,code);
                code = "@nv," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+34,col+i,code);
              }
          }catch(com.f1j.util.F1Exception e1){
          }
        }
//      }
    }else if(jRb3.isSelected()){
      for(i=0;i<vecselgl.size();i++){
        String str_1 = (String)vecselgl.get(i);
          try{
            if(jCB.isSelected()){//横向
              if(i==0){
                 for(j=col;j<col+12;j++){
                     usacreport.FrmSacReport.jBook1.setText(row-1,j,Integer.toString(j-col+1));
                   }
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+12,"最大值");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+13,"发生时间");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+14,"最小值");
                   usacreport.FrmSacReport.jBook1.setText(row-1,col+15,"发生时间");
                 }
                 usacreport.FrmSacReport.jBook1.setText(row+i,col-1,(String)vecselgl.get(i));//描述
                 code = "@ys," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col,code);
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+1,code);
                 code = "@zs," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+12,code);
                 code = "@zt," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+13,code);
                 code = "@zu," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+14,code);
                 code = "@zv," + str_1 + ",0";
                 usacreport.FrmSacReport.jBook1.setText(row+i,col+15,code);
            }
            else{
              if(i==0){
                for(j=row;j<row+12;j++){
                    usacreport.FrmSacReport.jBook1.setText(j,col-1,Integer.toString(j-row+1));
                  }
                  usacreport.FrmSacReport.jBook1.setText(row+12,col-1,"最大值");
                  usacreport.FrmSacReport.jBook1.setText(row+13,col-1,"发生时间");
                  usacreport.FrmSacReport.jBook1.setText(row+14,col-1,"最小值");
                  usacreport.FrmSacReport.jBook1.setText(row+15,col-1,"发生时间");
                }
                usacreport.FrmSacReport.jBook1.setText(row-1,col+i,(String)vecselgl.get(i));//描述
                code = "@ys," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row,col+i,code);
                usacreport.FrmSacReport.jBook1.setText(row+1,col+i,code);
                code = "@zs," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+12,col+i,code);
                code = "@zt," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+13,col+i,code);
                code = "@zu," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+14,col+i,code);
                code = "@zv," + str_1 + ",0";
                usacreport.FrmSacReport.jBook1.setText(row+15,col+i,code);
              }
          }catch(com.f1j.util.F1Exception e1){
          }
        }
//     }
    }
    vecselgl.removeAllElements();
    this.hide();*/
  }
  void dayfill(int mon,int id)
{
  int i;String ii=null;
  if(id == 0){
    jComdayb.removeAllItems();
  }else{
    jComdaye.removeAllItems();
  }

  switch(mon)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
    {
      if(id == 0){
        for(i=1;i<32;i++)
        {
          ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
            jComdayb.addItem(ii);
        }
      }else{
        for(i=1;i<32;i++)
        {
          ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
            jComdaye.addItem(ii);
        }
      }
      break;
    }
    case 4:
    case 6:
    case 9:
    case 11:
    {
      if(id == 0){
        for(i=1;i<31;i++)
        {
          ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
          jComdayb.addItem(ii);
        }
      }else{
        for(i=1;i<31;i++)
        {
          ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
            jComdaye.addItem(ii);
        }
      }
      break;
    }
    case 2:
    {
      if(id == 0){
        if((jComyearb.getSelectedIndex()+2000)%4!=0)
        {
          for(i=1;i<29;i++)
            {
            ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdayb.addItem(ii);
            }
        }
        else  if((jComyearb.getSelectedIndex()+2000)%100!=0)
        {
            for(i=1;i<30;i++)
            {
              ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdayb.addItem(ii);
            }
        }
        else  if((jComyearb.getSelectedIndex()+2000)%400!=0)
        {
            for(i=1;i<29;i++)
            {
              ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdayb.addItem(ii);
            }
        }
        else
        {
            for(i=1;i<30;i++)
            {
              ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdayb.addItem(ii);
            }
        }
      }else{
        if((jComyeare.getSelectedIndex()+2000)%4!=0)
        {
          for(i=1;i<29;i++)
            {
            ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdaye.addItem(ii);
            }
        }
        else  if((jComyeare.getSelectedIndex()+2000)%100!=0)
        {
            for(i=1;i<30;i++)
            {
              ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdaye.addItem(ii);
            }
        }
        else  if((jComyeare.getSelectedIndex()+2000)%400!=0)
        {
            for(i=1;i<29;i++)
            {
              ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdaye.addItem(ii);
            }
        }
        else
        {
            for(i=1;i<30;i++)
            {
              ii=Integer.toString(i);
          if (Integer.parseInt(ii)<10)  ii="0"+ii;
                jComdaye.addItem(ii);
            }
        }
      }
    }
  }

//    fill_cal();
  }
  void jComyearb_actionPerformed(ActionEvent e) {
   dayfill(jCommonb.getSelectedIndex()+1,0);
  }
  void jCommonb_actionPerformed(ActionEvent e) {
  dayfill(jCommonb.getSelectedIndex()+1,0);
  }
  void jComyeare_actionPerformed(ActionEvent e) {
   dayfill(jCommone.getSelectedIndex()+1,1);
  }
  void jCommone_actionPerformed(ActionEvent e) {
   dayfill(jCommone.getSelectedIndex()+1,1);
  }
  void jButRm_actionPerformed(ActionEvent e) {
    String s1 = "1970-01-01";
    String jct = jChanget.getText();
    String tablename="yc"+jct.substring(0,4)+jct.substring(5,7);

    SimpleDateFormat formatter = new SimpleDateFormat ("yyyy-MM-dd");
    ParsePosition pos = new ParsePosition(0);
    java.util.Date dt1=formatter.parse(s1,pos);

    ParsePosition pos3 = new ParsePosition(0);
    java.util.Date dt4=formatter.parse(jct,pos3);
    long l3 = (dt4.getTime() - dt1.getTime())/(3600*24*1000);

    String dbname = "modeldb";
    String where = "sdate=" + l3 + " and name='SsysjxloadP' and flag = 51";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());

    if(returnv > 0){
      int m_ret = usacreport.FrmSacReport.f1jdll.removeARecord(where.getBytes());
      usacreport.FrmSacReport.f1jdll.closeTableInfo();
      JOptionPane.showMessageDialog(null,
                   "删除检修计划成功！",
                   "OK",
                              JOptionPane.INFORMATION_MESSAGE);
    }
  }
}
