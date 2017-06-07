package com.daqo.dbinfo;

import javax.swing.*;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Properties;

/**
 * Created by 洪祥 on 2015/3/31.
 * 检索数据库相关表的相关字段的重复项
 */
public class ExecuteVerifyProcess extends Thread{

    Dlllink dlllink = Dlllink.getInstance();

    private VerifyProgressBar parentDialog;

    ExecuteVerifyProcess(VerifyProgressBar dialog) {
        parentDialog = dialog;
        startdb();
    }

    public void run() {
        parentDialog.setButtonEnabled(false);

        File file = new File(Dlllink.runhome + "/log/" ,"数据库表校验结果.txt");

        try {
            file.createNewFile();
            BufferedWriter bWriter = new BufferedWriter(new FileWriter(file));

            //写入前置通道值班机校验结果
            insertVerifyOutputSeparator(bWriter,"前置通道值班机校验");
            verifyChannelParaNotIn(bWriter);
            //写入数据库相关表重复项校验结果
            insertVerifyOutputSeparator(bWriter,"数据库重复项校验 ");
            verifyBbParaRepeat(bWriter);
            //写入数据库相关表代码规则校验结果
//            insertVerifyOutputSeparator(bWriter,"数据库代码规则校验");
//            verifyCodeRegulation(bWriter);

            bWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        parentDialog.dispose();

        JOptionPane.showMessageDialog(null, "校验成功！输出文件：["+ file.getAbsolutePath() +"]！", "校验结果",
                JOptionPane.CLOSED_OPTION);

        //校验完成时自动打开文件
        openFile(file);
    }

    //校验完成，打开文件
    private void openFile(File file){
        try {
            Properties props = System.getProperties(); //获得系统属性集
            String osName = props.getProperty("os.name"); //获得操作系统名称
            if (osName.startsWith("Windows")){
                //当前系统是Windows,在CMD上执行命令
                Runtime.getRuntime().exec("cmd.exe /c start " + file.getAbsolutePath());
            }else {
                //其他Linux平台下打开文件命令
                Runtime.getRuntime().exec("gedit " + file.getAbsolutePath());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //前置值班机校验
    private void verifyChannelParaNotIn(BufferedWriter bWriter) throws IOException {
        parentDialog.setBarText("正在校验前置通道值班机...");
        try {
            sleep(300); //考虑到此项校验一般记录很少，很快就执行完成，所以在此处加了延时，一共900毫秒，显示校验了这一项，优化人机交互体验
            parentDialog.setBarValue(30);
            sleep(300);
            parentDialog.setBarValue(60);
            sleep(300);
            parentDialog.setBarValue(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        writeChannelParaNotIn(this,bWriter);
        bWriter.newLine();
    }

    //数据库相关表重复项校验
    private void verifyBbParaRepeat(BufferedWriter bWriter) throws IOException {
        //写入SCD设备参数表DEVID字段重复项
        parentDialog.setBarText("正在校验SCD设备参数表...");
        writeSCDParaRepeat(this,bWriter);
        bWriter.newLine();

        //写入终端参数表终端序号字段重复项
        parentDialog.setBarText("正在校验终端参数表...");
        writeTerminalParaRepeat(this, bWriter);
        bWriter.newLine();

        //写入通道参数表通道序号字段重复项
        parentDialog.setBarText("正在校验通道参数表...");
        writeChannelParaRepeat(this,bWriter);
        bWriter.newLine();

        //写入遥测参数表遥测序号字段重复项
        parentDialog.setBarText("正在校验遥测参数表...");
        writeTelemetryParaRepeat(this,bWriter);
        bWriter.newLine();

        //写入遥信参数表遥信序号字段重复项
        parentDialog.setBarText("正在校验遥信参数表...");
        writeTelesignalParaRepeat(this,bWriter);
        bWriter.newLine();

        //写入电度参数表电度序号字段重复项
        parentDialog.setBarText("正在校验电度参数表...");
        writeKwhParaRepeat(this,bWriter);
        bWriter.newLine();
    }

    //数据库代码规则校验
    private void verifyCodeRegulation(BufferedWriter bWriter) throws IOException {
        //写入SCD设备参数表代码规则校验结果
        parentDialog.setBarText("正在校验SCD设备参数代码规则...");
        writeSCDDevCodeRegulation(bWriter);
        bWriter.newLine();
        //写入遥测参数表代码规则校验结果
        parentDialog.setBarText("正在校验遥测参数代码规则...");
        writeYCParaCodeRegulation(bWriter);
        bWriter.newLine();
        //写入遥信参数表代码规则校验结果
        parentDialog.setBarText("正在校验遥信参数代码规则...");
        writeYXParaCodeRegulation(bWriter);
        bWriter.newLine();
        //写入电度参数表代码规则校验结果
        parentDialog.setBarText("正在校验电度参数代码规则...");
        writeKWHParaCodeRegulation(bWriter);
        bWriter.newLine();
    }

    //写入SCD设备参数表DEVID字段重复项
    void writeSCDParaRepeat( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "------";

        byte[][] buf = instance.querySCDParaTable();

        bWriter.write("SCD设备参数表");
        bWriter.newLine();
        bWriter.write("DEVID" + space + "代码" + space + "描述");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator);
        bWriter.newLine();

        for(int i = 0;i<buf.length; i=i+3){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String str = dlllink.substr(buf[i]) + space + dlllink.substr(buf[i+1]) + space + dlllink.substr(buf[i+2]);
            bWriter.write(str);
            bWriter.newLine();
        }
    }

    //写入终端参数表终端序号字段重复项
    void writeTerminalParaRepeat( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "------";

        byte[][] buf = instance.queryTerminalParaTable();

        bWriter.write("终端参数表");
        bWriter.newLine();
        bWriter.write("终端序号" + space + "代码" + space + "描述");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator);
        bWriter.newLine();

        for(int i = 0;i<buf.length; i=i+3){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + dlllink.substr(buf[i+2]);
            bWriter.write(str);
            bWriter.newLine();
        }
    }

    //写入通道参数表通道序号字段重复项
    void writeChannelParaRepeat( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "------";

        byte[][] buf = instance.queryChannelParaTable();

        bWriter.write("通道参数表");
        bWriter.newLine();
        bWriter.write("通道序号" + space + "代码" + space + "描述");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator);
        bWriter.newLine();

        for(int i = 0;i<buf.length; i=i+3){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + dlllink.substr(buf[i+2]);
            bWriter.write(str);
            bWriter.newLine();
        }
    }

    //写入遥测参数表遥信序号字段重复项
    void writeTelemetryParaRepeat( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "--------";

        bWriter.write("遥测参数表");
        bWriter.newLine();
        bWriter.write("所属厂站" + space + "遥测序号" + space +"设备代码" + space + "描述");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] tmpbuf = instance.queryTerminalParaInfo();    //查找所有厂站
        //分厂站检索遥测序号字段重复项
        for(int i = 0;i<tmpbuf.length; i=i+2){
            float tmp = ((float)i)/tmpbuf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            byte[][] buf = instance.queryTelemetryParaTable(Integer.parseInt(dlllink.substr(tmpbuf[i])));
            for(int j = 0; j < buf.length; j=j+3){
                String str = dlllink.substr(tmpbuf[i+1]) + space + dlllink.substr(buf[j]) + space + space + dlllink.substr(buf[j+1]) + space + dlllink.substr(buf[j+2]);
                bWriter.write(str);
                bWriter.newLine();
            }
            if (buf.length > 0){
                bWriter.newLine();
            }
        }
    }

    //写入遥信参数表遥信序号字段重复项
    void writeTelesignalParaRepeat( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "--------";

        bWriter.write("遥信参数表");
        bWriter.newLine();
        bWriter.write("所属厂站" + space + "遥信序号" + space +"设备代码" + space + "描述");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] tmpbuf = instance.queryTerminalParaInfo();    //查找所有厂站
        //分厂站检索遥信序号字段重复项
        for(int i = 0;i<tmpbuf.length; i=i+2){
            float tmp = ((float)i)/tmpbuf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            byte[][] buf = instance.queryTelesignalParaTable(Integer.parseInt(dlllink.substr(tmpbuf[i])));
            for(int j = 0; j < buf.length; j=j+3){
                String str = dlllink.substr(tmpbuf[i+1]) + space + dlllink.substr(buf[j]) + space + space + dlllink.substr(buf[j+1]) + space + dlllink.substr(buf[j+2]);
                bWriter.write(str);
                bWriter.newLine();
            }
            if (buf.length > 0){
                bWriter.newLine();
            }
        }
    }

    //写入遥信参数表遥信序号字段重复项
    void writeKwhParaRepeat( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "--------";

        bWriter.write("电度参数表");
        bWriter.newLine();
        bWriter.write("所属厂站" + space + "电度序号" + space +"设备代码" + space + "描述");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] tmpbuf = instance.queryTerminalParaInfo();    //查找所有厂站
        //分厂站检索遥信序号字段重复项
        for(int i = 0;i<tmpbuf.length; i=i+2){
            float tmp = ((float)i)/tmpbuf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            byte[][] buf = instance.queryKwhParaTable(Integer.parseInt(dlllink.substr(tmpbuf[i])));
            for(int j = 0; j < buf.length; j=j+3){
                String str = dlllink.substr(tmpbuf[i+1]) + space + dlllink.substr(buf[j]) + space + space + dlllink.substr(buf[j+1]) + space + dlllink.substr(buf[j+2]);
                bWriter.write(str);
                bWriter.newLine();
            }
            if (buf.length > 0){
                bWriter.newLine();
            }
        }
    }

    //写入通道参数表默认值班机字段记录不在组服务器配置参数表服务器类型为FEP或PUBFEP对应节点名字段中的记录（妈呀好拗口）
    void writeChannelParaNotIn( ExecuteVerifyProcess instance,BufferedWriter bWriter) throws IOException {

        String space = "  ";
        String separator = "--------";
        bWriter.write("通道参数表");
        bWriter.newLine();
        bWriter.write("通道序号" + space + "描述" + space +"默认值班机" );
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] buf = instance.queryChannelParaNotInTable();
        //输出结果
        for(int i = 0;i<buf.length; i=i+3){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + dlllink.substr(buf[i+2]);
            bWriter.write(str);
            bWriter.newLine();
        }
    }

    //写入SCD设备参数表代码规则校验结果
    void writeSCDDevCodeRegulation(BufferedWriter bWriter) throws IOException {

        String space = "   ";
        String separator = "--------";
        bWriter.write("SCD设备参数表");
        bWriter.newLine();
//        bWriter.write("说明：代码 = 类型（4位） + 站所线系代码（5位）+ 5位随机码。以下列出的是不符合规则的记录。");
//        bWriter.newLine();
//        bWriter.newLine();
        bWriter.write("代码" + space + space + "描述" + space + space + "站所线系代码" + space + "类型" );
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表", "代码,描述,站所线系代码,类型", "", "", new int[1], false);

        for(int i = 0;i < buf.length; i+=4){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String code = dlllink.substr(buf[i]);
            String type = convertType(dlllink.substr(buf[i + 3]));
            String subCode = dlllink.substr(buf[i + 2]);
            if (code.length() < 14){
                //如果代码长度小于14位，这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + dlllink.substr(buf[i+1]) + space + dlllink.substr(buf[i+2]) + space + dlllink.substr(buf[i+3]);
                bWriter.write(str);
                bWriter.newLine();
            }else if (!code.substring(0,9).equals(type + subCode)){
                //如果代码前9位不等于类型和站所线系的组合，这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + dlllink.substr(buf[i+1]) + space + dlllink.substr(buf[i+2]) + space + dlllink.substr(buf[i+3]);
                bWriter.write(str);
                bWriter.newLine();
            }
        }

    }

    //写入遥测参数表代码规则校验结果
    void writeYCParaCodeRegulation(BufferedWriter bWriter) throws IOException {

        String space = "     ";
        String separator = "--------";
        bWriter.write("遥测参数表");
        bWriter.newLine();
//        bWriter.write("说明：代码 = 设备代码（14位） +  5位随机码 + 类型（4位）。以下列出的是不符合规则的记录。");
//        bWriter.newLine();
//        bWriter.newLine();
        bWriter.write("终端序号" + space +  "遥测序号" + space + "代码" + space + space  + space + space + space + "描述" +
                space + space + space + "设备代码" + space + "类型" + space + space + "子类型" + space + "原因");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space +separator + separator + separator + space + separator +
                space + space + separator + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥测参数表", "终端序号,遥测序号,代码,描述,设备代码,类型,子类型", "", "终端序号", new int[1], false);

        for(int i = 0;i < buf.length; i+=7){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String code = dlllink.substr(buf[i+2]);
            String type = convertType(dlllink.substr(buf[i + 5]));
            String devCode = dlllink.substr(buf[i + 4]);
            String subType = dlllink.substr(buf[i + 6]);
            if (code.length() < 23){
                //如果遥测代码小于23位，这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) +
                        space + space + dlllink.substr(buf[i+6])  + space + "代码长度不符";
                bWriter.write(str);
                bWriter.newLine();
            }else if (!code.substring(0,14).equals(devCode) || !code.substring(19,23).equals(type)){
                //如果遥测代码前14位不等于设备代码，或者代码后4位不等于类型，那么这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) +
                        space + space + dlllink.substr(buf[i+6])  + space + "代码不符设备代码或类型要求";
                bWriter.write(str);
                bWriter.newLine();
            }else if((code.substring(14,19).equals("00000") && !subType.equals("0")) || (!code.substring(14,19).equals("00000") && subType.equals("0"))){
                //校验遥测代码第15位到第19位这五位与子类型是否匹配
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) +
                        space + space + dlllink.substr(buf[i+6])  + space + "代码不符子类型要求";
                bWriter.write(str);
                bWriter.newLine();
            }
        }
    }

    //写入遥信参数表代码规则校验结果
    void writeYXParaCodeRegulation(BufferedWriter bWriter) throws IOException {

        String space = "     ";
        String separator = "--------";
        bWriter.write("遥信参数表");
        bWriter.newLine();
//        bWriter.write("说明：代码 = 设备代码（14位） +  5位随机码 + 类型（4位）。以下列出的是不符合规则的记录。");
//        bWriter.newLine();
//        bWriter.newLine();
        bWriter.write("终端序号" + space +  "遥信序号" + space + "代码" + space + space  + space + space + space + "描述" +
                space + space +  space + "设备代码" + space + "类型" + space + space + "原因");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator + separator + separator + space + separator +
                space + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥信参数表", "终端序号,遥信序号,代码,描述,设备代码,类型", "", "终端序号", new int[1], false);

        for(int i = 0;i < buf.length; i+=6){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String code = dlllink.substr(buf[i+2]);
            String type = convertType(dlllink.substr(buf[i + 5]));
            String devCode = dlllink.substr(buf[i + 4]);
            int iType = Integer.parseInt(type);
            if (code.length() < 23){
                //如果遥信代码小于23位，这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) + space + "代码长度不符";
                bWriter.write(str);
                bWriter.newLine();
            }else if (!code.substring(0,14).equals(devCode) || !code.substring(19,23).equals(type)){
                //如果遥信代码前14位不等于设备代码，或者代码后4位不等于类型，那么这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) + space + "代码不符设备代码或类型要求";
                bWriter.write(str);
                bWriter.newLine();
            }else if(((iType >= 300 && iType <= 399) || (iType >= 500 && iType <= 599)) &&  !code.substring(14,19).equals("00000")){
                //如果设备类型处于300到399或500到599之间，但是代码第15位到第19位不等于00000
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) + space + "代码不符类型要求";
                bWriter.write(str);
                bWriter.newLine();
            }
        }
    }

    //写入电度参数表代码规则校验结果
    void writeKWHParaCodeRegulation(BufferedWriter bWriter) throws IOException {

        String space = "     ";
        String separator = "--------";
        bWriter.write("电度参数表");
        bWriter.newLine();
//        bWriter.write("说明：代码 = 设备代码（14位） +  5位随机码 + 类型（4位）。以下列出的是不符合规则的记录。");
//        bWriter.newLine();
//        bWriter.newLine();
        bWriter.write("终端序号" + space +  "电度序号" + space + "代码" + space + space  + space + space + space + "描述" +
                space + space +  space + "设备代码" + space + "类型" + space + space + "原因");
        bWriter.newLine();
        bWriter.write(separator + space + separator + space + separator + separator + separator + space + separator +
                space + space + separator + space + separator + space + separator);
        bWriter.newLine();

        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "电度参数表", "终端序号,电度序号,代码,描述,设备代码,类型", "", "终端序号", new int[1], false);

        for(int i = 0;i < buf.length; i+=6){
            float tmp = ((float)i)/buf.length;
            int progressValue = (int)(tmp*100);
            parentDialog.setBarValue(progressValue);
            String code = dlllink.substr(buf[i+2]);
            String type = convertType(dlllink.substr(buf[i + 5]));
            String devCode = dlllink.substr(buf[i + 4]);
            if (code.length() < 23){
                //如果电度代码小于23位，这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) + space + "代码长度不符";
                bWriter.write(str);
                bWriter.newLine();
            }else if (!code.substring(0,14).equals(devCode) || !code.substring(19,23).equals(type)){
                //如果电度代码前14位不等于设备代码，或者代码后4位不等于类型，那么这条记录有误，输出
                String str = dlllink.substr(buf[i]) + space + space + dlllink.substr(buf[i+1]) + space + space + dlllink.substr(buf[i+2]) +
                        space + dlllink.substr(buf[i+3]) + space + dlllink.substr(buf[i+4]) + space + dlllink.substr(buf[i+5]) + space + "代码不符设备代码或类型要求";
                bWriter.write(str);
                bWriter.newLine();
            }
        }
    }

    //检索SCD设备参数表devid字段重复记录
    public byte[][] querySCDParaTable(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "devid in (select devid from SCD设备参数表 group by devid having count(*)>1)";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表",
                "devid,代码,描述", wh, "devid", fieldcount, false);
        return buf;
    }

    //检索终端参数表终端序号字段重复记录
    public byte[][] queryTerminalParaTable(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "终端序号 in (select 终端序号 from 终端参数表 group by 终端序号 having count(*)>1)";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                "终端序号,代码,描述", wh, "终端序号", fieldcount, false);
        return buf;
    }

    //检索通道参数表通道序号字段重复记录
    public byte[][] queryChannelParaTable(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "通道序号 in (select 通道序号 from 通道参数表 group by 通道序号 having count(*)>1)";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "通道参数表",
                "通道序号,代码,描述", wh, "通道序号", fieldcount, false);
        return buf;
    }

    //搜索终端参数表，为后面分厂站检索遥测参数表、遥信参数表和电度参数表的相关字段重复记录做准备
    public byte[][] queryTerminalParaInfo(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                "终端序号,描述", "", "终端序号", fieldcount, false);
        return buf;
    }

    //分厂站检索遥测参数表遥测序号字段重复记录
    public byte[][] queryTelemetryParaTable(int terminalNo){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "遥测序号 in (select 遥测序号 from 遥测参数表 where 终端序号 =" + terminalNo + "group by 遥测序号 having count(*)>1) and 终端序号 =" + terminalNo;
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥测参数表",
                "遥测序号,设备代码,描述", wh, "遥测序号", fieldcount, false);
        return buf;
    }

    //分厂站检索遥信参数表遥信序号字段重复记录
    public byte[][] queryTelesignalParaTable(int terminalNo){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "遥信序号 in (select 遥信序号 from 遥信参数表 where 终端序号 =" + terminalNo + "group by 遥信序号 having count(*)>1) and 终端序号 =" + terminalNo;
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥信参数表",
                "遥信序号,设备代码,描述", wh, "遥信序号", fieldcount, false);
        return buf;
    }

    //分厂站检索电度参数表电度序号字段重复记录
    public byte[][] queryKwhParaTable(int terminalNo){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "电度序号 in (select 电度序号 from 电度参数表 where 终端序号 =" + terminalNo + "group by 电度序号 having count(*)>1) and 终端序号 =" + terminalNo;
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "电度参数表",
                "电度序号,设备代码,描述", wh, "电度序号", fieldcount, false);
        return buf;
    }

    //搜索通道参数表默认值班机字段记录不在组服务器配置参数表服务器类型为FEP或PUBFEP对应节点名字段中的记录（妈呀好拗口）
    public byte[][] queryChannelParaNotInTable(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "默认值班机 not in (select 节点名 from 组服务器配置参数表 where 服务器类型 = 'FEP' or 服务器类型 = 'PUBFEP')";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "通道参数表",
                "通道序号,描述,默认值班机", wh, "通道序号", fieldcount, false);
        return buf;
    }

    //将读取的类型转换为符合代码规则的字符串
    private String convertType(String type){
        int i = Integer.parseInt(type);
        if (i < 10){
            type = "000" + i;
        }else if(i < 100){
            type = "00" + i;
        }else if(i < 1000){
            type = "0" + i;
        }else {
            type = "" + i;
        }
        return type;
    }

    //生成一段校验项间隔符说明
    private void insertVerifyOutputSeparator(BufferedWriter bWriter,String directions) throws IOException {
        String separator = "**********************************************************";
        String space = "*                                                        *";
        bWriter.write(separator);
        bWriter.newLine();
        bWriter.write(space);
        bWriter.newLine();
        directions = "*                   "  + directions + "                    *";
        bWriter.write(directions);
        bWriter.newLine();
        bWriter.write(space);
        bWriter.newLine();
        bWriter.write(separator);
        bWriter.newLine();
    }

    //连接数据库
    private void startdb(){
        try {
            Dlllink.dbstatus = dlllink.startdb();
        }
        catch (UnsatisfiedLinkError e) {
            JOptionPane.showMessageDialog(null, "动态库dblink连接失败!", "发送信息提示", JOptionPane.ERROR_MESSAGE);
        }
    }

    //关闭数据库
    public void closedb(){
        dlllink.closedb();
    }
}
