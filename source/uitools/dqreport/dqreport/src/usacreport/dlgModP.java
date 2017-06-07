package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.util.Calendar;
import java.util.Date;
import java.util.Vector;
import java.util.Random;
import java.awt.BorderLayout;
import javax.swing.border.EmptyBorder;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class dlgModP extends JDialog {
  private JPanel panel1 = new JPanel();
  private XYLayout xYLayout1 = new XYLayout();
  private JLabel jLabel1 = new JLabel();
  private JLabel jLabel2 = new JLabel();
  private JTextField jTextora = new JTextField();
  private JTextField jTextmod = new JTextField();
  private JButton jButmod = new JButton();
  private JButton jButcan = new JButton();
  private JButton jButPre = new JButton();
  private JButton jButNext = new JButton();
  private JEditorPane jwhere = new JEditorPane();
  private int pos = 0;
  private JLabel jLabel3 = new JLabel();
  private JTextField jTextmod1 = new JTextField();
  private int returnv = 0;
  private Vector vecpos = new Vector();
  private Random dataran = new Random();
  private String base="";
  private String base1="";
  private Random dataran1 = new Random();
  private String strwhere = "";
  private JLabel jLabel4 = new JLabel();
   public dlgModP(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgModP() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    panel1.setLayout(xYLayout1);
    panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("原始值");
    panel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("上限值");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jButmod.setText("修改");
    jButmod.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButmod_actionPerformed(e);
      }
    });
    jButcan.setText("退出");
    jButcan.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButcan_actionPerformed(e);
      }
    });
    jButPre.setText("上一个");
    jButPre.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButPre_actionPerformed(e);
      }
    });
    jButNext.setText("下一个");
    jButNext.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButNext_actionPerformed(e);
      }
    });
    jwhere.setEnabled(false);
    jwhere.setFont(new java.awt.Font("Serif", 0, 12));
    jwhere.setMinimumSize(new Dimension(600, 29));
    jTextora.setEnabled(false);
    jTextora.setFont(new java.awt.Font("SansSerif", 0, 12));
    jTextmod.setFont(new java.awt.Font("SansSerif", 0, 12));
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("下限值");
    jTextmod1.setFont(new java.awt.Font("SansSerif", 0, 12));
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("当前值");
      panel1.add(jLabel1,    new XYConstraints(44, 41, 54, 26));
    panel1.add(jLabel2,       new XYConstraints(44, 86, 55, 26));
    panel1.add(jTextmod,  new XYConstraints(103, 88, 147, 26));
    panel1.add(jTextora, new XYConstraints(103, 43, 147, 25));
      panel1.add(jButmod, new XYConstraints(49, 300, 97, 28));
    panel1.add(jLabel3,    new XYConstraints(44, 131, 61, 26));
    panel1.add(jTextmod1,  new XYConstraints(103, 133, 147, 26));
      panel1.add(jButcan, new XYConstraints(222, 300, 97, 28));
      panel1.add(jButPre, new XYConstraints(44, 250, 97, 28));
      panel1.add(jButNext, new XYConstraints(153, 249, 97, 28));
      panel1.add(jwhere, new XYConstraints(103, 168, 148, 64));
      panel1.add(jLabel4, new XYConstraints(44, 5, 108, 26));
      this.getContentPane().add(panel1, java.awt.BorderLayout.EAST);

    if(pos == 0) jButPre.setEnabled(false);
    if(pos == usacreport.dlgModData.vectoridarr.size()/5) jButNext.setEnabled(false);
  }

  void opentableinfo()
  {
    returnv = usacreport.FrmSacReport.f1jdll.openTableInfo("modeldb".getBytes(),usacreport.dlgModData.str_dbname.getBytes());
    if(returnv < 0) System.out.println("读取table信息失败!");
  }

  void closetableinfo()
  {
    if(returnv > 0) usacreport.FrmSacReport.f1jdll.closeTableInfo();
  }

  void jButPre_actionPerformed(ActionEvent e) {
    OnPrec();
  }

  void jButNext_actionPerformed(ActionEvent e) {
    OnNext();
  }

  void jButmod_actionPerformed(ActionEvent e) {
    if(jTextmod.getText().trim().compareTo("")==0 || jTextmod.getText().trim()==null) return;
    if(jTextmod1.getText().trim().compareTo("")==0||jTextmod1.getText().trim()==null) return;
    if(base.compareTo(jTextmod.getText().trim())!=0)
    {
      dataran.setSeed((long)(Long.parseLong(jTextmod.getText().trim())/20));
      base = jTextmod.getText().trim();
    }
    else if(base1.compareTo(jTextmod1.getText().trim())!=0)
    {
      dataran1.setSeed((long)(Long.parseLong(jTextmod1.getText().trim())/20));
      base1 = jTextmod1.getText().trim();
    }
    else
    {
      dataran.setSeed((long)(1.00/20));
      base = jTextmod.getText().trim();
      dataran1.setSeed((long)(1.00/20));
      base1 = jTextmod1.getText().trim();
    }

    byte [][] buffer = new byte[5][0];
    int []acount = new int[5];//name,sdate,time,data
    String str_data="";
    acount[0] = 0;
    acount[1] = 1;
    acount[2] = 2;
    acount[3] = 3;
    acount[4] = 4;

    buffer[0] = ((String)usacreport.dlgModData.vectoridarr.get(pos*5)).getBytes();
    buffer[1] = ((String)usacreport.dlgModData.vectoridarr.get(pos*5+1)).getBytes();
    buffer[2] = ((String)usacreport.dlgModData.vectoridarr.get(pos*5+2)).getBytes();
    buffer[3] = ((String)usacreport.dlgModData.vectoridarr.get(pos*5+3)).getBytes();
    buffer[4] = ((String)usacreport.dlgModData.vectoridarr.get(pos*5+4)).getBytes();
    if(Double.parseDouble(jTextora.getText().trim()) > Double.parseDouble(jTextmod.getText().trim()))
    {
       str_data = Float.toString(dataran.nextFloat()+Float.parseFloat(jTextmod.getText().trim()));
      buffer[4] = str_data.getBytes();
    }
    else if(Double.parseDouble(jTextora.getText().trim()) < Double.parseDouble(jTextmod1.getText().trim()))
    {
      str_data = Float.toString(Float.parseFloat(jTextmod1.getText().trim())-dataran1.nextFloat());
      buffer[4] = str_data.getBytes();;
    }

    if(usacreport.FrmSacReport.f1jdll.updateARecord(buffer,acount,3) < 0)
    {
      System.out.println("更新数据失败!");
    }
    else
    {
      if(vecpos.size() == 0)
      {
        vecpos.addElement(Integer.toString(pos));
      }
      else
      {
        boolean bcunzai = false;
        for(int i=0;i<vecpos.size();i++)
        {
          if(Integer.toString(pos).compareTo(vecpos.get(i).toString())==0)
          {
          bcunzai = true;
          break;
          }
        }
        if(!bcunzai)
        {
          vecpos.addElement(Integer.toString(pos));
        }
      }
      if(Double.parseDouble(jTextora.getText().trim()) > Double.parseDouble(jTextmod.getText().trim()))
      {
        usacreport.dlgModData.vectoridarr.set(pos*5+4,(Object)str_data);
      }
      else if(Double.parseDouble(jTextora.getText().trim()) < Double.parseDouble(jTextmod1.getText().trim()))
      {
        usacreport.dlgModData.vectoridarr.set(pos*5+4,(Object)str_data);
      }
    }
    if(jButNext.isEnabled())
    {
      OnNext();
    }
    else
    {
      pos=0;
      inittext();
    }
  }

  void jButcan_actionPerformed(ActionEvent e) {

//    System.out.println(Integer.toString(JOptionPane.showConfirmDialog(this,"是否要进行统计")));
    if(JOptionPane.showConfirmDialog(this,"是否要进行统计")==0)
    {
      for(int i=0;i<vecpos.size();i++)
      {
        Calendar caltemp1 = Calendar.getInstance();
        caltemp1.set(Calendar.ZONE_OFFSET, 28800000);

        Date dttemp = new Date();
        long ltemp = Long.parseLong((String)usacreport.dlgModData.vectoridarr.get((Integer.parseInt((String)vecpos.get(i)))*5+1));
        ltemp = ltemp * (long)86400000.0;
        dttemp.setTime(ltemp);
        caltemp1.setTime(dttemp);
        String strsdate = Integer.toString(caltemp1.get(caltemp1.YEAR));
        if(caltemp1.get(caltemp1.MONTH)+1 < 10)
        {
          strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
        }
        else
        {
          strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
        }
        if(caltemp1.get(caltemp1.DAY_OF_MONTH)<10)
        {
          strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
        }
        else
        {
          strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
        }

        if(usacreport.dlgModData.vectoridarr.size()!=0)
        {
          String tempname = (String)usacreport.dlgModData.vectoridarr.get(0);
          String procname  = "exec data_statics" + usacreport.dlgModData.str_dbname.substring(2,usacreport.dlgModData.str_dbname.length());
         // String sql = procname + " (" + strsdate + "," + "1," + "'" + tempname +"')";
          String sql = procname + " " + strsdate + "," + "1," + "'" + tempname+"'";
          System.out.println(sql);
          int returnv = usacreport.FrmSacReport.f1jdll.ExecuteCmd("modeldb".getBytes(),sql.getBytes());
          System.out.println(Integer.toString(returnv));
          if(returnv<=0)
          {
            System.out.println("执行统计过程失败!!");
          }
        }
      }
    }
    vecpos.removeAllElements();
    closetableinfo();
    this.hide();
  }

  void OnNext()
  {
    jButPre.setEnabled(true);
    pos++;
    if(pos == usacreport.dlgModData.vectoridarr.size()/5-1) jButNext.setEnabled(false);
    jTextora.setText((String)usacreport.dlgModData.vectoridarr.get(pos*5+4));

    Calendar caltemp1 = Calendar.getInstance();
    caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
    Date dttemp = new Date();
    long ltemp = Long.parseLong((String)usacreport.dlgModData.vectoridarr.get(pos*5+1));
    ltemp = ltemp * (long)86400000.0;
    dttemp.setTime(ltemp);
    caltemp1.setTime(dttemp);
    String strsdate = Integer.toString(caltemp1.get(caltemp1.YEAR));
    if(caltemp1.get(caltemp1.MONTH)+1 < 10)
    {
      strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
    }
    else
    {
      strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
    }
    if(caltemp1.get(caltemp1.DAY_OF_MONTH)<10)
    {
      strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
    }
    else
    {
      strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
    }
    strwhere = "name = '"+(String)usacreport.dlgModData.vectoridarr.get(pos*5) + "'  and sdate = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+1)
                   +"  and time = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+2) + " and flag = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+3);
    jwhere.setText("name = '"+(String)usacreport.dlgModData.vectoridarr.get(pos*5) + "'  年月日 = " + strsdate
                   +"  小时 = " + Integer.toString(Integer.parseInt((String)usacreport.dlgModData.vectoridarr.get(pos*5+2))/60) + "点 flag = 整点值");
  }

  void OnPrec()
  {
    jButNext.setEnabled(true);
    pos--;
    if(pos == 0) jButPre.setEnabled(false);
    jTextora.setText((String)usacreport.dlgModData.vectoridarr.get(pos*5+4));

    Calendar caltemp1 = Calendar.getInstance();
    caltemp1.set(Calendar.ZONE_OFFSET, 28800000);
    Date dttemp = new Date();
    long ltemp = Long.parseLong((String)usacreport.dlgModData.vectoridarr.get(pos*5+1));
    ltemp = ltemp * (long)86400000.0;
    dttemp.setTime(ltemp);
    caltemp1.setTime(dttemp);
    String strsdate = Integer.toString(caltemp1.get(caltemp1.YEAR));
    if(caltemp1.get(caltemp1.MONTH)+1 < 10)
    {
      strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
    }
    else
    {
      strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
    }
    if(caltemp1.get(caltemp1.DAY_OF_MONTH)<10)
    {
      strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
    }
    else
    {
      strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
    }
    strwhere = "name = '"+(String)usacreport.dlgModData.vectoridarr.get(pos*5) + "'  and sdate = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+1)
                   +"  and time = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+2) + " and flag = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+3);
    jwhere.setText("name = '"+(String)usacreport.dlgModData.vectoridarr.get(pos*5) + "'  年月日 = " + strsdate
                   +"  小时 = " + Integer.toString(Integer.parseInt((String)usacreport.dlgModData.vectoridarr.get(pos*5+2))/60) + "点 flag = 整点值");
//    jwhere.setText("name = '"+(String)usacreport.dlgModData.vectoridarr.get(pos*5) + "'  and sdate = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+1)
//                   +"  and time = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+2) + " and flag = " + (String)usacreport.dlgModData.vectoridarr.get(pos*5+3));
  }

  boolean inittext()
  {
    if(usacreport.dlgModData.vectoridarr.size()==0)
    {
      return false;
    }
    if(usacreport.dlgModData.str_dbname.length()!=8)
    {
      return false;
    }
    pos = 0;
    if(pos == 0)
    {
      jButPre.setEnabled(false);
    }
    else{jButPre.setEnabled(true);}

    if(pos == usacreport.dlgModData.vectoridarr.size()/5-1)
    {
      jButNext.setEnabled(false);
    }
    else{jButNext.setEnabled(true);}
    if(usacreport.dlgModData.vectoridarr.size() > 0)
    {
      jTextora.setText((String)usacreport.dlgModData.vectoridarr.get(4));

      Calendar caltemp1 = Calendar.getInstance();
      caltemp1.set(Calendar.ZONE_OFFSET, 28800000);

      Date dttemp = new Date();
      long ltemp = Long.parseLong((String)usacreport.dlgModData.vectoridarr.get(1));
      ltemp = ltemp * (long)86400000.0;
      dttemp.setTime(ltemp);
      caltemp1.setTime(dttemp);
      String strsdate = Integer.toString(caltemp1.get(caltemp1.YEAR));
      if(caltemp1.get(caltemp1.MONTH)+1 < 10)
      {
        strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
      }
      else
      {
        strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.MONTH)+1);
      }
      if(caltemp1.get(caltemp1.DAY_OF_MONTH)<10)
      {
        strsdate = strsdate + "0" + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
      }
      else
      {
        strsdate = strsdate + Integer.toString(caltemp1.get(caltemp1.DAY_OF_MONTH));
      }
      strwhere = "name = '"+(String)usacreport.dlgModData.vectoridarr.get(0) + "'  and sdate = " + (String)usacreport.dlgModData.vectoridarr.get(1)
               +"  and time = " + (String)usacreport.dlgModData.vectoridarr.get(2) + " and flag = " + (String)usacreport.dlgModData.vectoridarr.get(3);
      jwhere.setText("name = '"+(String)usacreport.dlgModData.vectoridarr.get(pos*5) + "'  年月日 = " + strsdate
                     +"  小时 = " + Integer.toString(Integer.parseInt((String)usacreport.dlgModData.vectoridarr.get(pos*5+2))/60) + "点 flag = 整点值");
//      jwhere.setText("name = '"+(String)usacreport.dlgModData.vectoridarr.get(0) + "'  and sdate = " + (String)usacreport.dlgModData.vectoridarr.get(1)
//                     +"  and time = " + (String)usacreport.dlgModData.vectoridarr.get(2) + " and flag = " + (String)usacreport.dlgModData.vectoridarr.get(3));
    }
    return true;
  }
}
