package usacreport;

import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.Calendar;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.FileOutputStream;
import java.net.UnknownHostException;
import java.net.InetAddress;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public final class PreProcessor {
   public final static boolean SpecialCmdLine  =  true;    //表示命令行前面和后面带字符\的情况
   public final static boolean ExportExcelFile =  true;    //表示导出报表文件到Excel中
   public final static boolean SQL_DEBUG       =  false;   //输出调试信息到单元格中,如果未定义,默认输出的是空格
   public final static boolean CMDLINE_DEBUG   =  true;    //写日志文件和调试
   public final static boolean UseSelHourTime  =  false;   //使用了开始时间，如果是日表示开始的时间，如果是月则是某日的某点。
   public final static boolean PWTEST          =  false;   //配网测试用，将报表中的小时自动设置为21点。
   public final static boolean MD5             =  false;    //MD5加密

   final static Logger log = Logger.getLogger("gem");
   public PreProcessor() {
      log.setLevel(Level.FINER);
      ConsoleHandler c1 = new ConsoleHandler();
      c1.setLevel(Level.FINER);
      log.addHandler(c1);
   }

   //format:主机名，用户名，操作
   public static void WriteOptLogFile(String logMsg)
   {
      String hostname = "";
      try {
        InetAddress neta = InetAddress.getLocalHost();
        hostname = neta.getHostName();
      }
      catch (UnknownHostException nete) {
        hostname = "";
      }
      String user =   dlgusr.vectorusr.get(dlgusr.g_loginUserIdx+1).toString();
      String right =  dlgusr.vectorusr.get(dlgusr.g_loginUserIdx).toString();
      String dbglogstr = "主机名：" + hostname.trim() + "  操作员：" + user.trim() + "  类别:" + right + "   ";
      dbglogstr += logMsg;
      WriteLogFile(dbglogstr);
   }

   public static void WriteLogFile(String logMsg){

      String filename = new String(FrmSacReport.f1jdll.getenvvar()) + "/log/daqoreportlog.txt";
      File file = new File(filename);
      long filelength = file.length();

      if (filelength > (1024 * 1024 * 12)) { //4M
         file.delete();
      }

      if (file.exists() == false) {
         try {
            file.createNewFile();
         }
         catch (IOException ioe) {
            ioe.printStackTrace();
         }
      }
      try {
         FileOutputStream fos = new FileOutputStream(file, true);
         PrintWriter pw = new PrintWriter(fos);
         Calendar calendar = Calendar.getInstance();
         calendar.set(Calendar.ZONE_OFFSET, 28800000);

         int a = calendar.get(Calendar.AM_PM);
         int hour = calendar.get(Calendar.HOUR);
         if (a == 1) { //下午
            hour += 12;
         }

         int mon = calendar.get(Calendar.MONTH) + 1;
         String date = calendar.get(Calendar.YEAR) + "-" + Integer.toString(mon) +
             "-" + Integer.toString(calendar.get(Calendar.DATE))
             + "  " + Integer.toString(hour) + ":" +
             Integer.toString(calendar.get(Calendar.MINUTE))
             + ":" + Integer.toString(calendar.get(Calendar.SECOND)) + "     " + "  " + logMsg;
         pw.println(date);
         pw.close();

      }
      catch (Exception fne) {
         fne.printStackTrace();
      }
   }
}
