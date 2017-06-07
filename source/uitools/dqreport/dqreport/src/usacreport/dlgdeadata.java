package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Vector;
import java.util.*;
import java.text.*;
import javax.swing.*;
import java.awt.*;
import javax.swing.border.*;
import java.awt.event.*;


/*import jxl.*;
import jxl.format.UnderlineStyle;
import jxl.write.*;
import jxl.write.Number;
import jxl.write.Boolean;*/

import java.io.*;
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Dimension;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgdeadata extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JButton jButquit = new JButton();
  javax.swing.ButtonGroup bg = new javax.swing.ButtonGroup();
  private JComboBox jComFac = new JComboBox();
  private JComboBox jComTime = new JComboBox();
  private JLabel jLabel3 = new JLabel();
  private JScrollPane jScrollPane1 = new JScrollPane();
  private JList jListall = new JList();
  private JButton jButOk = new JButton();
  public static Vector vecselgl = new Vector();
  private Vector vecid = new Vector();
  private Vector checkbox = new Vector();
  private ArrayList alldata = new ArrayList();
   private dlgEvent dlg =null;
  public static Vector vectoridarr = new Vector();
  private Vector temp = new Vector();
  private Vector temp1 = new Vector();
  private Vector temp2 = new Vector();
  private Vector temp3 = new Vector();
  private Vector temp4 = new Vector();
  private Vector temp5 = new Vector();
  String strr=null;
  public dlgdeadata(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgdeadata() {
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
    Date currTime = new Date();
    SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd",Locale.CHINA);
    strr=new String(formatter.format(currTime).getBytes("iso-8859-1"));

    panel1.setLayout(xYLayout1);
   // panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
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
    jButOk.setText("统计");
    jButOk.setFont(new java.awt.Font("Dialog", 0, 12));
    jButOk.setActionCommand("jButquit");
    jListall.setFont(new java.awt.Font("SansSerif", 0, 12));
    panel1.setMinimumSize(new Dimension(360, 320));
    jButton1.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButton1.setBorder(null);
      jButton1.setPreferredSize(new Dimension(29, 29));
    jButton1.setToolTipText("");
      jButton1.setActionCommand("From");
      jButton1.setBorderPainted(false);
      jButton1.setText("From");
    jButton1.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButton1_actionPerformed(e);
      }
    });
    jButton2.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButton2_actionPerformed(e);
      }
    });
    jButton2.setText("To");
    jButton2.setToolTipText("");
      jButton2.setBorderPainted(false);
      jButton2.setMnemonic('0');
      jButton2.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jButton2.setBorder(null);
      jButton2.setMaximumSize(new Dimension(29, 19));
      jButton2.setMinimumSize(new Dimension(29, 19));
      jButton2.setPreferredSize(new Dimension(29, 29));
    jtimeb.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
    jtimee.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
    jtimeb.setText(strr);
    jtimee.setText(strr);
    jComTime.setFont(new java.awt.Font("Dialog", 0, 12));
    jComTime.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComTime_actionPerformed(e);
      }
    });
    jComTime.addItem("五分钟");
    jComTime.addItem("一小时");
      jComFac.setFont(new java.awt.Font("Dialog", 0, 12));

      panel1.setPreferredSize(new Dimension(360, 320));
      panel1.add(jComFac, new XYConstraints(75, 7, 127, 28));
      panel1.add(jtimeb, new XYConstraints(67, 241, 96, 29));
      this.getContentPane().add(panel1, java.awt.BorderLayout.CENTER);
      panel1.add(jtimee, new XYConstraints(208, 241, 99, 30));
      panel1.add(jButOk, new XYConstraints(21, 284, 88, 31));
      panel1.add(jButquit, new XYConstraints(212, 284, 89, 30));
      panel1.add(jComTime, new XYConstraints(224, 7, 83, 28));
      panel1.add(jLabel3, new XYConstraints(22, 8, 50, 27));
      panel1.add(jScrollPane1, new XYConstraints(23, 49, 282, 172));
      panel1.add(jButton1, new XYConstraints(22, 241, 45, -1));
      panel1.add(jButton2, new XYConstraints(163, 241, 45, -1));
      jScrollPane1.getViewport().add(jListall);

    Calendar cal1 = Calendar.getInstance();
    cal1.set(Calendar.ZONE_OFFSET, 28800000);

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
        String str_2 = new String(buffer[i+1]);//描述
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
 /* public static void writeExcel(OutputStream os) throws Exception {
    WritableWorkbook wwb = Workbook.createWorkbook(os);
    WritableSheet ws = wwb.createSheet("TestSheet1", 0);

    jxl.write.Label labelC = new jxl.write.Label(0, 0, "我爱中国1111");
    ws.addCell(labelC);
    WritableFont wfc =
        new WritableFont(
        WritableFont.ARIAL,
        20,
        WritableFont.BOLD,
        false,
        UnderlineStyle.NO_UNDERLINE,
        jxl.format.Colour.GREEN);
    WritableCellFormat wcfFC = new WritableCellFormat(wfc);
    wcfFC.setBackground(jxl.format.Colour.RED);
    labelC = new jxl.write.Label(6, 0, "中国爱我love", wcfFC);
    ws.addCell(labelC);


    jxl.write.NumberFormat nf = new jxl.write.NumberFormat("#.##");
    WritableCellFormat wcfN = new WritableCellFormat(nf);
    jxl.write.Number labelNF = new jxl.write.Number(1,1,3.1415926,wcfN);
    ws.addCell(labelNF);

    //写入Exel工作表
    wwb.write();
    //关闭Excel工作薄对象
    wwb.close();
  }*/
/*  public static void readExcel(String filePath)
  {
    try
    {
      int i,j;
      String strc00=null;
      InputStream is = new FileInputStream(filePath);
      Workbook rwb = Workbook.getWorkbook(is);
      Sheet st = rwb.getSheet(0);//这里有两种方法获取sheet表,1为名字，而为下标，从0开始
      int row = st.getRows();
      int col = st.getColumns();

      //File fileWrite = new File(filePath);
      //OutputStream os = new FileOutputStream(fileWrite);
      for(i=0;i<col;i++){
          for(j=0;j<row;j++){
              Cell c00 = st.getCell(i,j);
             if( c00.getType()==CellType.EMPTY)   break;
              if(c00.getType() == CellType.LABEL)
                {
                  LabelCell labelc00 = (LabelCell)c00;
                  strc00 = labelc00.getString();
                }
                //输出
             // System.out.println(strc00);
                if(strc00.substring(0,1).compareTo("1") == 0){
                  strc00="OK";
                }
                System.out.println(strc00);
          }
      }
      rwb.close();
    }
    catch(Exception e)
    {
      e.printStackTrace();
    }
  }*/

  public String daytodate(long daynum){
    int year=0;
    int month=0;
    int day=0;

    String datestr="";
    Date d=new Date((long)daynum*24*3600*1000);
    java.util.Calendar ds=java.util.Calendar.getInstance();
    ds.setTime(d);
    year=ds.get(java.util.Calendar.YEAR);
    month=ds.get(java.util.Calendar.MONTH);
    day=ds.get(java.util.Calendar.DATE);
    datestr=Integer.toString(year)+"-"+Integer.toString(1+month)+"-"+Integer.toString(day);
    return datestr;
  }

  void jButOk_actionPerformed(ActionEvent e) {
    insertdate();
  }
  int i=0;
  Date currTime = new Date();
  private JButton jButton1 = new JButton();
  private JButton jButton2 = new JButton();
  private JTextField jtimeb = new JTextField();
  private JTextField jtimee = new JTextField();
  SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd",Locale.CHINA);

  void insertdate(){
    temp.clear();
    temp1.clear();
    temp2.clear();
    temp3.clear();
    temp4.clear();
    temp5.clear();
//user.readExcel("d:/bb.XLS");
    try{
    //InputStream is = new FileInputStream("d:/x.XLS");
 //   readExcel("d:\\t.XLS");
   // fileWrite.createNewFile();
   // OutputStream os = new FileOutputStream(fileWrite);
   // writeExcel(os);
    }catch(Exception e){

    }

    String dbname = "modeldb";
    String tablename = "";
    String fieldname = "";
    String where = "",order = "",code = "";
    int records  = 0;
    int[] icount = new int[1];
    vectoridarr.removeAllElements();
   // String yearb=(String)jComyearb.getSelectedItem();
   // String monb=(String)jCommonb.getSelectedItem();
   // String dayb=(String)jComdayb.getSelectedItem();
    String timeb=(String)jtimeb.getText();
  //  String yeare=(String)jComyeare.getSelectedItem();
  //  String mone=(String)jCommone.getSelectedItem();
  //  String daye=(String)jComdaye.getSelectedItem();
    String timee=(String)jtimee.getText();
    String s1 = "1970-01-01";
  //  String tb=yearb+"-"+monb+"-"+dayb;
    String  tb=timeb;
  //  String te=yeare+"-"+mone+"-"+daye;
    String  te=timee;

    ParsePosition pos = new ParsePosition(0);
    ParsePosition posb = new ParsePosition(0);
    ParsePosition pose = new ParsePosition(0);
    java.util.Date dt1=formatter.parse(s1,pos);
    java.util.Date dtb=formatter.parse(tb,posb);
    java.util.Date dte=formatter.parse(te,pose);
    long l1 = (dtb.getTime() - dt1.getTime())/(3600*24*1000);
    long l2 = (dte.getTime() - dt1.getTime())/(3600*24*1000);

    tablename="yc"+tb.substring(0,4)+tb.substring(5,7);
    String name=null;
    String data="0";
   // Object[] objsel = jListall.getSelectedValues();
    if(jListall.getSelectedIndex() < 0) {
      for(i=0;i<vecid.size();i++){
        name=(String)vecid.get(i);
        where = "name = '" + name + "' and sdate >=" + l1 + " and sdate <= "+l2+" and flag < 50 and mod(time,60)=0";
        records = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
            order.getBytes(),icount,true);
        if(records <= 0){
          JOptionPane.showMessageDialog(this,"没有符合条件的记录!!");
          return;
        }
        buffer = new byte[records][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
        String str=null;
        int n=0;
        for(int ii=0;ii<records;ii+=6){
          String str2 = new String(buffer[ii+1]);
          str2 = str2.trim();
          temp4.addElement(str2);
          String str3 = new String(buffer[ii+2]);
          str3 = str3.trim();
          temp1.addElement(str3);
          String str4 = new String(buffer[ii+4]);
          str4 = str4.trim();
          temp.addElement(str4);
        }

        int aa=0;
        while(aa<(temp.size()-1))
        {
          int flag=1;
          for(int i=aa+1;i<temp.size();i++){
            String a=(String)temp.get(i);
            String b=(String)temp.get(aa);
            if (a.equals(b)){
              flag++;
              if (i==temp.size()-1)
              {
                if(flag>=3){
                  String t=(String)temp1.get(aa);
                  for (int k=0;k<flag;k++)
                  {
                    temp2.addElement(temp.get(aa));
                    temp3.addElement(String.valueOf(Integer.parseInt(t)+60*k));
                    temp5.addElement(temp4.get(aa));
                  }
                }
                aa=temp.size();
              }
            }else{
              if(flag>=3){
                String t=(String)temp1.get(aa);
                for (int k=0;k<flag;k++)
                {
                  temp2.addElement(temp.get(aa));
                  temp3.addElement(String.valueOf(Integer.parseInt(t)+60*k));
                  temp5.addElement(temp4.get(aa));
                }
              }
              aa=i;
              break;
            }
          }
        }
        for(int i=0;i<temp2.size();i++){
          try
          {
          String t=(String)temp3.get(i);
          String d=(String)temp5.get(i);
          d=daytodate(Long.parseLong(d));
          int tt=Integer.parseInt(t);
          int a=tt/60;
          int b=tt%60;
          usacreport.FrmSacReport.jBook1.setMaxRow(temp2.size());
          //  usacreport.FrmSacReport.jBook1.setMaxCol(temp2.size());
          usacreport.FrmSacReport.jBook1.setText(0,0,"代码");
          usacreport.FrmSacReport.jBook1.setText(0,1,"时间");
          usacreport.FrmSacReport.jBook1.setText(0,2,"值");
          usacreport.FrmSacReport.jBook1.setText(i+1,0,name);
          usacreport.FrmSacReport.jBook1.setText(i+1,1,String.valueOf(d)+" "+String.valueOf(a)+":"+String.valueOf(b));
          usacreport.FrmSacReport.jBook1.setText(i+1,2,(String)temp2.get(i));
        }
        catch( com.f1j.util.F1Exception e1){
        }
        }
      }
    }else{
      int [] select=jListall.getSelectedIndices();
      for (int m=0;m<select.length;m++){
        CheckListItem  item = new CheckListItem(false,(String)vecid.get(select[m]),i++);
        name = item.getText();
        int time=jComTime.getSelectedIndex();
        if (time==0){
          time=5;
        }else{
          time=60;
        }
        where = "name = '"+name+"' and sdate >=" + l1 + " and sdate <= "+l2+" and flag < 50 and mod(time,"+time+")=0";
        records = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
            order.getBytes(),icount,true);
        if(records <= 0){
          JOptionPane.showMessageDialog(this,"没有符合条件的记录!!");
          return;
        }
        buffer = new byte[records][usacreport.ConstantDef.MAX_COLUMN_LEN];
        int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
        String str=null;
        int n=0;
        for(int ii=0;ii<records;ii+=6){
          //  String str1 = new String(buffer[i]);
          //  str1 = str1.trim();
          String str2 = new String(buffer[ii+1]);
          str2 = str2.trim();
          temp4.addElement(str2);
          String str3 = new String(buffer[ii+2]);
          str3 = str3.trim();
          temp1.addElement(str3);
          String str4 = new String(buffer[ii+4]);
          str4 = str4.trim();
          temp.addElement(str4);
        }
        int aa=0;
        while(aa<(temp.size()-1))
        {
          int flag=1;
          for(int i=aa+1;i<temp.size();i++){
            String a=(String)temp.get(i);
            String b=(String)temp.get(aa);
            if (a.equals(b)){
              flag++;
              if (i==temp.size()-1)
              {
                if(flag>=3){
                  String t=(String)temp1.get(aa);
                  for (int k=0;k<flag;k++)
                  {
                    temp2.addElement(temp.get(aa));
                    temp3.addElement(String.valueOf(Integer.parseInt(t)+time*k));
                    temp5.addElement(temp4.get(aa));
                  }
                }
                aa=temp.size();
              }
            }else{
              if(flag>=3){
                String t=(String)temp1.get(aa);
                for (int k=0;k<flag;k++)
                {
                  temp2.addElement(temp.get(aa));
                  temp3.addElement(String.valueOf(Integer.parseInt(t)+time*k));
                  temp5.addElement(temp4.get(aa));
                }
              }
              aa=i;
              break;
            }
          }
        }
        for(int i=0;i<temp2.size();i++){
          try
          {
          String t=(String)temp3.get(i);
          String d=(String)temp5.get(i);
          d=daytodate(Long.parseLong(d));
          int tt=Integer.parseInt(t);
          int a=tt/60;
          int b=tt%60;
          usacreport.FrmSacReport.jBook1.setMaxRow(temp2.size());
          //  usacreport.FrmSacReport.jBook1.setMaxCol(temp2.size());
          usacreport.FrmSacReport.jBook1.setText(0,0,"代码");
          usacreport.FrmSacReport.jBook1.setText(0,1,"时间");
          usacreport.FrmSacReport.jBook1.setText(0,2,"值");
          usacreport.FrmSacReport.jBook1.setText(i+1,0,name);
          usacreport.FrmSacReport.jBook1.setText(i+1,1,String.valueOf(d)+" "+String.valueOf(a)+":"+String.valueOf(b));
          usacreport.FrmSacReport.jBook1.setText(i+1,2,(String)temp2.get(i));
        }
        catch( com.f1j.util.F1Exception e1){
        }
        }
      }
    }
    temp.clear();
    temp1.clear();
    temp2.clear();
    temp3.clear();
    temp4.clear();
    temp5.clear();
    this.hide();
  }
  DateChooser dc = new DateChooser(this,true);
  DateChooser dc1 = new DateChooser(this,true);


  void jButton1_actionPerformed(ActionEvent e) {
    dc.setLocationRelativeTo(jtimeb);
    dc.setVisible(true);
    java.util.Calendar cal = dc.getSelectedDate();
    if (cal != null) {
      jtimeb.setText(formatter.format(cal.getTime()));
    }
  }
  void jButton2_actionPerformed(ActionEvent e) {
    dc1.setLocationRelativeTo(jtimee);
    dc1.setVisible(true);
    java.util.Calendar cal = dc1.getSelectedDate();
    if (cal != null) {
      jtimee.setText(formatter.format(cal.getTime()));
    }
  }
  void jComTime_actionPerformed(ActionEvent e) {

  }
}
