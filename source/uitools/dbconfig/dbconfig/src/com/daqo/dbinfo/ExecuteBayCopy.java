package com.daqo.dbinfo;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

/**
 * Created by 洪祥 on 2015/7/13.
 */
public class ExecuteBayCopy extends Thread{

    Dlllink dlllink = Dlllink.getInstance();
    private BayCopyDialog parentDialog;

    ExecuteBayCopy(BayCopyDialog dialog) {
        parentDialog = dialog;
    }

    public void run() {
        beforeCopyInit();
        executeBayCopy();
        finishCopyInit();
    }

    //执行复制之前，对复制对话框进行设置
    private void beforeCopyInit(){
        parentDialog.process.setText("正在执行复制...");
        parentDialog.process.setForeground(Color.blue);
        parentDialog.progressbar.setVisible(true);
        parentDialog.subCopy_from.setEnabled(false);
        parentDialog.subCopy_to.setEnabled(false);
        parentDialog.terminalCopy_from.setEnabled(false);
        parentDialog.terminalCopy_to.setEnabled(false);
        parentDialog.checkBox_YC.setEnabled(false);
        parentDialog.checkBox_YX.setEnabled(false);
        parentDialog.checkBox_KWH.setEnabled(false);
        parentDialog.jBtOK.setEnabled(false);
    }

    //完成复制后，提示用户并退出
    private void finishCopyInit(){
        parentDialog.process.setText("");
        setProgressbarValue(100);
        parentDialog.subCopy_from.setEnabled(true);
        parentDialog.subCopy_to.setEnabled(true);
        parentDialog.terminalCopy_from.setEnabled(true);
        parentDialog.terminalCopy_to.setEnabled(true);
        parentDialog.jBtOK.setEnabled(true);
        parentDialog.checkBox_YC.setEnabled(true);
        parentDialog.checkBox_YX.setEnabled(true);
        parentDialog.checkBox_KWH.setEnabled(true);
        JOptionPane.showMessageDialog(null, "复制完成！", "提示信息", JOptionPane.INFORMATION_MESSAGE);
        parentDialog.dispose();
    }

    //顺序执行间隔复制
    private void executeBayCopy(){

        int scdParaNum = queryParaInDbCount("SCD设备参数表","站所线系代码",
                getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString()));
        //首先判断目标厂站的SCD设备参数表是否为空，如果为空，说明是第一次复制某个厂站下的终端，则执行SCD设备参数复制。
        if (scdParaNum == 0){
            executeSCDDevParaCopy();
        }
        executeYCParaCopy();
        executeYXParaCopy();
        executeKWHParaCopy();
    }

    //执行SCD设备参数复制
    private void executeSCDDevParaCopy(){
        byte[][] scd_records = this.getSCDParaInfoBySub(parentDialog.subCopy_from.getSelectedItem().toString());
        //获取SCD设备参数表域的个数
        int scd_field_count = this.openTableInfo(ConstantDef.MODELDB, "SCD设备参数表");
        //外层循环，逐条向SCD设备参数表插入记录
        for(int i = 0; i < scd_records.length; i += scd_field_count){
            byte[][] scd_value = new byte[scd_field_count][0];
            //内层循环，逐个字段赋值，形成一条完整的记录
            for(int j = 0; j < scd_field_count; j++){
                if(j == 4){ //更换SCD设备参数表代码字段
                    String code = this.subStr(scd_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetCode = code.replaceAll(subCodeFrom,subCodeTo);
                    scd_value[j] = targetCode.getBytes();
                }else if(j == 5){ //更新描述
                    String desc = this.subStr(scd_records[i + j]);
                    String subDescFrom = parentDialog.subCopy_from.getSelectedItem().toString();
                    if (desc.contains(subDescFrom)){
                        String subDescTo = parentDialog.subCopy_to.getSelectedItem().toString();
                        String targetDesc = desc.replaceAll(subDescFrom,subDescTo);
                        scd_value[j] = targetDesc.getBytes();
                    }else {
                        scd_value[j] = desc.getBytes();
                    }
                }else if(j == 6){ //更换SCD设备参数表站所线系代码字段
                    String targetSubCode = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    scd_value[j] = targetSubCode.getBytes();
                }else if(j == 18){ //如果所属馈线字段不为空，替换该字段的值
                    String feeder = this.subStr(scd_records[i + j]);
                    if(feeder.length() != 0){
                        String begin = feeder.substring(0,0);
                        String mid = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                        String end = feeder.substring((begin + mid).length(),feeder.length());
                        String targetFeeder = begin + mid + end;
                        scd_value[j] = targetFeeder.getBytes();
                    }
                } else{
                    scd_value[j] = scd_records[i+j];
                }
            }
            //插入一条记录
            this.insertARecord(ConstantDef.MODELDB,"SCD设备参数表",scd_value);
            int progressValue = (int)((((float)i)/scd_records.length)*33);
            setProgressbarValue(progressValue);
        }
        this.closeTableInfo();
    }

    //执行遥测参数复制
    private void executeYCParaCopy(){
        byte[][] yc_records = this.getYCParaInfoByTerminal(parentDialog.terminalCopy_from.getSelectedItem().toString());
        //获取遥测参数表域的个数
        int yc_field_count = this.openTableInfo(ConstantDef.MODELDB, "遥测参数表");
        //外层循环，逐条向遥测参数表插入记录
        for(int i = 0; i < yc_records.length; i += yc_field_count){
            byte[][] yc_value = new byte[yc_field_count][0];
            //内层循环，逐个字段赋值，形成一条完整的记录
            for(int j = 0; j < yc_field_count; j++){
                if(j == 3){ //更换遥测参数表代码字段
                    String code = this.subStr(yc_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetCode = code.replaceAll(subCodeFrom,subCodeTo);
                    yc_value[j] = targetCode.getBytes();
                }else if(j == 6){ //更换遥测参数表设备代码字段
                    String code = this.subStr(yc_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetDevCode = code.replaceAll(subCodeFrom,subCodeTo);
                    yc_value[j] = targetDevCode.getBytes();
                }else if(j == 9){ //更换遥测参数表终端序号字段
                    String terminalNo = this.getTerminalNoByDesc(parentDialog.terminalCopy_to.getSelectedItem().toString());
                    yc_value[j] = terminalNo.getBytes();
                } else{
                    yc_value[j] = yc_records[i + j];
                }
            }
            //插入一条记录
            this.insertARecord(ConstantDef.MODELDB,"遥测参数表",yc_value);
            int progressValue = 33 + (int)((((float)i)/yc_records.length)*33);
            setProgressbarValue(progressValue);
        }
        this.closeTableInfo();
    }

    //执行遥信参数复制
    private void executeYXParaCopy(){
        byte[][] yx_records = this.getYXParaInfoByTerminal(parentDialog.terminalCopy_from.getSelectedItem().toString());
        //获取遥信参数表域的个数
        int yx_field_count = this.openTableInfo(ConstantDef.MODELDB, "遥信参数表");
        //外层循环，逐条向遥信参数表插入记录
        for(int i = 0; i < yx_records.length; i += yx_field_count){
            byte[][] yx_value = new byte[yx_field_count][0];
            //内层循环，逐个字段赋值，形成一条完整的记录
            for(int j = 0; j < yx_field_count; j++){
                if(j == 3){ //更换遥信参数表代码字段
                    String code = this.subStr(yx_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetCode = code.replaceAll(subCodeFrom,subCodeTo);
                    yx_value[j] = targetCode.getBytes();
                }else if(j == 6){ //更换遥信参数表设备代码字段
                    String code = this.subStr(yx_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetDevCode = code.replaceAll(subCodeFrom, subCodeTo);
                    yx_value[j] = targetDevCode.getBytes();
                }else if(j == 11){ //更换遥信参数表终端序号字段
                    String terminalNo = this.getTerminalNoByDesc(parentDialog.terminalCopy_to.getSelectedItem().toString());
                    yx_value[j] = terminalNo.getBytes();
                } else{
                    yx_value[j] = yx_records[i + j];
                }
            }
            //插入一条记录
            this.insertARecord(ConstantDef.MODELDB,"遥信参数表",yx_value);
            int progressValue = 66 + (int)((((float)i)/yx_records.length)*33);
            setProgressbarValue(progressValue);
        }
        this.closeTableInfo();
    }

    //执行电度参数复制
    private void executeKWHParaCopy(){
        byte[][] kwh_records = this.getKWHParaInfoByTerminal(parentDialog.terminalCopy_from.getSelectedItem().toString());
        //获取电度参数表域的个数
        int kwh_field_count = this.openTableInfo(ConstantDef.MODELDB, "电度参数表");
        //外层循环，逐条向电度参数表插入记录
        for(int i = 0; i < kwh_records.length; i += kwh_field_count){
            byte[][] kwh_value = new byte[kwh_field_count][0];
            //内层循环，逐个字段赋值，形成一条完整的记录
            for(int j = 0; j < kwh_field_count; j++){
                if(j == 3){ //更换电度参数表代码字段
                    String code = this.subStr(kwh_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetCode = code.replaceAll(subCodeFrom,subCodeTo);
                    kwh_value[j] = targetCode.getBytes();
                }else if(j == 6){ //更换电度参数表设备代码字段
                    String code = this.subStr(kwh_records[i + j]);
                    String subCodeFrom = this.getSubCodeByDesc(parentDialog.subCopy_from.getSelectedItem().toString());
                    String subCodeTo = this.getSubCodeByDesc(parentDialog.subCopy_to.getSelectedItem().toString());
                    String targetDevCode = code.replaceAll(subCodeFrom,subCodeTo);
                    kwh_value[j] = targetDevCode.getBytes();
                }else if(j == 8){ //更换电度参数表终端序号字段
                    String terminalNo = this.getTerminalNoByDesc(parentDialog.terminalCopy_to.getSelectedItem().toString());
                    kwh_value[j] = terminalNo.getBytes();
                } else{
                    kwh_value[j] = kwh_records[i + j];
                }
            }
            //插入一条记录
            this.insertARecord(ConstantDef.MODELDB,"电度参数表",kwh_value);
            int progressValue = 99 + (int)((((float)i)/kwh_records.length)*33);
            setProgressbarValue(progressValue);
        }
        this.closeTableInfo();
    }

    //设置进度条进度
    private void setProgressbarValue(int value){
        parentDialog.progressbar.setValue(value);
    }

    //检索站所线系参数表，获取厂站
    private byte[][] querySubParaInfo(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "站所线系参数表",
                "代码,描述", "", "描述", fieldcount, false);
        return buf;
    }

    //检索终端参数表，获取终端信息
    private byte[][] queryTerminalParaInfo(){
        byte[][] buf;
        int[] fieldcount = new int[1];
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                "代码,描述", "", "终端序号", fieldcount, false);
        return buf;
    }

    //检索数据库某张表某个字段在数据库中相同值的个数
    private int queryParaInDbCount(String table,String field,String value){

        int[] fieldcount = new int[1];
        String wh = field + " = '" + value + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, table,
                field, wh, "终端序号", fieldcount, false);
        return buf.length;

    }

    //根据描述获取厂站代码
    public String getSubCodeByDesc(String desc){
        int[] fieldcount = new int[1];
        String wh = "描述" + " = '" + desc + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "站所线系参数表",
                "代码", wh, "", fieldcount, false);
        return dlllink.substr(buf[0]);
    }

    //根据描述获取终端序号
    private String getTerminalNoByDesc(String desc){
        int[] fieldcount = new int[1];
        String wh = "描述" + " = '" + desc + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                "终端序号", wh, "", fieldcount, false);
        return dlllink.substr(buf[0]);
    }

    //获取可以作为源长站的厂站名
    public ArrayList<String> getSourceSub(){
        ArrayList<String> sourceSub = new ArrayList<String>();
        //获取所有厂站信息
        byte[][] subInfo = querySubParaInfo();
        for(int i = 0;i<subInfo.length; i=i+2){
            int num = queryParaInDbCount("SCD设备参数表", "站所线系代码", dlllink.substr(subInfo[i]));
            String str = dlllink.substr(subInfo[i+1]);
            //只有有设备的厂站才能作为源厂站
            if(num > 0){
                sourceSub.add(str);
            }
        }
        return sourceSub;
    }

    //获取目标厂站的厂站名
    public ArrayList<String> getTargetSub(){
        ArrayList<String> targetSub = new ArrayList<String>();
        //获取所有厂站信息
        byte[][] subInfo = querySubParaInfo();
        for(int i = 0;i<subInfo.length; i=i+2){
            int num = queryParaInDbCount("SCD设备参数表", "站所线系代码", dlllink.substr(subInfo[i]));
            String str = dlllink.substr(subInfo[i+1]);
            //为了适应一个下有多个终端的情况，目标厂站非空也可以作为目标厂站
            if(num != -1 ){
                targetSub.add(str);
            }
        }
        return targetSub;
    }

    //获取可以作为源终端的终端名
    public ArrayList<String> getSourceTerminal(){
        ArrayList<String> sourceTerminal = new ArrayList<String>();
        //获取所有终端信息
        byte[][] terminalInfo = queryTerminalParaInfo();
        for(int i = 0;i<terminalInfo.length; i=i+2){
            sourceTerminal.add(dlllink.substr(terminalInfo[i+1]));
        }
        return sourceTerminal;
    }

    //获取可以作为目标终端的终端名
    public ArrayList<String> getTargetTerminal(){
        ArrayList<String> targetTerminal = new ArrayList<String>();
        //获取所有厂站信息
        byte[][] terminalInfo = queryTerminalParaInfo();
        for(int i = 0;i<terminalInfo.length; i=i+2){
            targetTerminal.add(dlllink.substr(terminalInfo[i+1]));
        }
        return targetTerminal;
    }

    //根据所选择的厂站，获取其对应的所有终端
    public ArrayList<String> getTerminalBySub(String sub){
        ArrayList<String> terminal = new ArrayList<String>();
        String wh = "站所线系代码" + " = '" + getSubCodeByDesc(sub) + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                "描述", wh, "", new int[1], false);
        for(int i = 0;i < buf.length; i++){
            terminal.add(dlllink.substr(buf[i]));
        }
        return terminal;
    }

    //检索SCD设备参数表，获取所有指定厂站的SCD设备
    public byte[][] getSCDParaInfoBySub(String sub){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "站所线系代码" + " = '" + getSubCodeByDesc(sub) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表",
                "*", wh, "", fieldcount, false);
        return buf;
    }

    //检索遥测参数表，获取所有指定终端序号的遥测点
    public byte[][] getYCParaInfoByTerminal(String terminal){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "终端序号" + " = '" + getTerminalNoByDesc(terminal) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥测参数表",
                "*", wh, "", fieldcount, false);
        return buf;
    }

    //检索遥信参数表，获取所有指定终端序号的遥信点
    public byte[][] getYXParaInfoByTerminal(String terminal){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "终端序号" + " = '" + getTerminalNoByDesc(terminal) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥信参数表",
                "*", wh, "", fieldcount, false);
        return buf;
    }

    //检索电度参数表，获取所有指定终端序号的电度
    public byte[][] getKWHParaInfoByTerminal(String terminal){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "终端序号" + " = '" + getTerminalNoByDesc(terminal) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "电度参数表",
                "*", wh, "", fieldcount, false);
        return buf;
    }

    //打开表信息
    public int openTableInfo(String db, String table){
        return dlllink.openTableInfo(db.getBytes(),table.getBytes());
    }

    //关闭表信息
    public void closeTableInfo(){
        dlllink.closeTableInfo();
    }

    //向指定库的某张表插入一条记录
    public int insertARecord(String db, String table, byte[][] value){
        openTableInfo(db,table);
        int ret =  dlllink.insertARecord(value);
        return ret;
    }

    //字节数组转String
    public String subStr(byte[] value){
        return dlllink.substr(value);
    }
}
