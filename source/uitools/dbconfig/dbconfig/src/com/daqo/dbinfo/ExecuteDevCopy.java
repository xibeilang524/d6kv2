package com.daqo.dbinfo;

import javax.swing.*;
import java.awt.*;
import java.awt.List;
import java.util.*;

/**
 * Created by 洪祥 on 2016/7/20.
 */
public class ExecuteDevCopy extends Thread{

    Dlllink dlllink = Dlllink.getInstance();
    private DevCopyDialog parentDialog;

    private String subCopy_from; //源厂站
    private String devCopy_from; //源设备
    private String subCopy_to; //目标厂站
    private String terminalCopy_to; //目标终端
    private ArrayList<String> devCopy_to = new ArrayList<String>(); //目标设备

    ExecuteDevCopy(DevCopyDialog dialog) {
        parentDialog = dialog;
    }

    public void run() {
        getInterfaceElem();
        beforeCopyInit();
        executeBayCopy();
        finishCopyInit();
    }

    //获取界面设置元素
    private  void getInterfaceElem(){
        subCopy_from = parentDialog.subCopy_from.getSelectedItem().toString();
        devCopy_from = parentDialog.devCopy_from.getSelectedItem().toString();
        subCopy_to = parentDialog.subCopy_to.getSelectedItem().toString();
        terminalCopy_to = parentDialog.terminalCopy_to.getSelectedItem().toString();

        //然后再根据复制模式，设置需要复制的目标设备
        if (parentDialog.checkBox_singleDevCopy.isSelected()){
            //单设备复制模式，目标设备直接设置为所选的设备
            devCopy_to.add(parentDialog.devCopy_to.getSelectedItem().toString());
        }else if(parentDialog.checkBox_allDevCopy.isSelected()){
            //全设备复制模式，将目标设备列表中所有的设备添加到要复制的设备列表中
            for (int i = 0; i < parentDialog.devCopy_to.getItemCount(); i++){
                devCopy_to.add(parentDialog.devCopy_to.getItemAt(i).toString());
            }
        }
    }
    //执行复制之前，对复制对话框进行设置
    private void beforeCopyInit(){
        parentDialog.process.setText("正在复制..");
        parentDialog.process.setForeground(Color.blue);
        parentDialog.subCopy_from.setEnabled(false);
        parentDialog.devCopy_from.setEnabled(false);
        parentDialog.subCopy_to.setEnabled(false);
        parentDialog.terminalCopy_to.setEnabled(false);
        parentDialog.devCopy_to.setEnabled(false);
        parentDialog.jBtOK.setEnabled(false);
    }

    //完成复制后，提示用户并退出
    private void finishCopyInit(){
        parentDialog.process.setText("");
        setProgressbarValue(100);
        parentDialog.subCopy_from.setEnabled(true);
        parentDialog.subCopy_to.setEnabled(true);
//        parentDialog.terminalCopy_from.setEnabled(true);
        parentDialog.terminalCopy_to.setEnabled(true);
        parentDialog.jBtOK.setEnabled(true);
        JOptionPane.showMessageDialog(null, "复制完成！", "提示信息", JOptionPane.INFORMATION_MESSAGE);
        parentDialog.restoreInterface();
//        parentDialog.dispose();
    }

    //顺序执行间隔复制
    private void executeBayCopy(){
        executeYCParaCopy();
        executeYXParaCopy();
        executeKWHParaCopy();
    }


    //执行遥测参数复制
    private void executeYCParaCopy(){
        byte[][] yc_records = this.getYCParaInfoBySubAndDevDesc(subCopy_from,devCopy_from); //全纪录
        //获取遥测参数表域的个数
        int yc_field_count = this.openTableInfo(ConstantDef.MODELDB, "遥测参数表");
        //外层循环，按个复制设备
        for (int i = 0; i < devCopy_to.size(); i++){
            //用于记录目标设备的第几条完整的记录。
            int calcRecordCount = 0;
            int nonCalcRecordCount = 0;
            //中层循环，逐条向遥测参数表插入记录
            int tableNonCalcRecordCount = getPointTableNonCalcRecordNum(terminalCopy_to,"遥测参数表"); //目标终端下挂的测点个数(非计算量)
            int tableCalcRecordCount = getPointTableCalcRecordNum(terminalCopy_to, "遥测参数表"); //目标终端下挂的测点个数(计算量)
            for(int j = 0;  j < yc_records.length;  j += yc_field_count){
                byte[][] yc_value = new byte[yc_field_count][0];
                //内层循环，逐个字段赋值，形成一条完整的记录
                for(int k = 0; k < yc_field_count; k++) {
                    if (k == 3) { //更换遥测参数表代码字段
                        String oldCode = this.subStr(yc_records[j + k]); //老代码
                        String newDevCode = getDevCodeBySubAndDevDesc(subCopy_to, devCopy_to.get(i)); //新设备代码
                        String oldDevCode = getDevCodeBySubAndDevDesc(subCopy_from, devCopy_from); //老设备代码
                        String targetCode = oldCode.replaceAll(oldDevCode,newDevCode); //将新设备代码替换老设备代码
                        yc_value[k] = targetCode.getBytes();
                    } else if(k == 4){ //更换遥测参数表描述字段
                        String oldDesc = this.subStr(yc_records[j + k]); //老描述
                        String newDesc = oldDesc.replaceAll(devCopy_from, devCopy_to.get(i)); //新设备描述替换老设备描述
                        yc_value[k] = newDesc.getBytes();
                    } else if (k == 6) { //更换遥测参数表设备代码字段
                        String newDevCode = getDevCodeBySubAndDevDesc(subCopy_to,devCopy_to.get(i));
                        yc_value[k] = newDevCode.getBytes();
                    } else if (k == 9) { //更换遥测参数表终端序号字段
                        String newTerminalNo = getTerminalNoByDesc(terminalCopy_to);
                        yc_value[k] = newTerminalNo.getBytes();
                    } else if (k == 10){ //更换遥测序号字段
                        String calc = this.subStr(yc_records[j + 30]); //获取计算量标志
                        int newYCNo = 0;
                        if (calc.equals("0")){
                            //非计算量
                            nonCalcRecordCount++;
                            newYCNo = (tableNonCalcRecordCount - 1)  + nonCalcRecordCount;
                        }else if (calc.equals("1")){
                            //计算量
                            calcRecordCount++;
                            int minCalcNo = getMinCalcNo(terminalCopy_to,"遥测参数表");
                            newYCNo = minCalcNo + (tableCalcRecordCount - 1) + calcRecordCount;
                        }
                        yc_value[k] = (newYCNo + "").getBytes();
                    } else {
                        yc_value[k] = yc_records[j + k];
                    }
                }
                //插入一条记录
                this.insertARecord(ConstantDef.MODELDB,"遥测参数表",yc_value);
                int progressValue = (int)((((float)i)/devCopy_to.size())*66);
                setProgressbarValue(progressValue);
            }
        }

        this.closeTableInfo();
    }

    //执行遥信参数复制
    private void executeYXParaCopy(){
        byte[][] yx_records = this.getYXParaInfoBySubAndDevDesc(subCopy_from, devCopy_from);
        //获取遥信参数表域的个数
        int yx_field_count = this.openTableInfo(ConstantDef.MODELDB, "遥信参数表");
        //外层循环，按个复制设备
        for (int i = 0; i < devCopy_to.size(); i++){
            //用于记录目标设备的第几条完整的记录。
            int calcRecordCount = 0;
            int nonCalcRecordCount = 0;
            //中层循环，逐条向遥信参数表插入记录
            int tableNonCalcRecordCount = getPointTableNonCalcRecordNum(terminalCopy_to,"遥信参数表"); //目标终端下挂的测点个数(非计算量)
            int tableCalcRecordCount = getPointTableCalcRecordNum(terminalCopy_to, "遥信参数表"); //目标终端下挂的测点个数(计算量)
            for(int j = 0;  j < yx_records.length;  j += yx_field_count){
                byte[][] yx_value = new byte[yx_field_count][0];
                //内层循环，逐个字段赋值，形成一条完整的记录
                for(int k = 0; k < yx_field_count; k++) {
                    if (k == 3) { //更换遥信参数表代码字段
                        String oldCode = this.subStr(yx_records[j + k]); //老代码
                        String newDevCode = getDevCodeBySubAndDevDesc(subCopy_to, devCopy_to.get(i)); //新设备代码
                        String oldDevCode = getDevCodeBySubAndDevDesc(subCopy_from,devCopy_from); //老设备代码
                        String targetCode = oldCode.replaceAll(oldDevCode,newDevCode); //将新设备代码替换老设备代码
                        yx_value[k] = targetCode.getBytes();
                    } else if(k == 4){ //更换遥信参数表描述字段
                        String oldDesc = this.subStr(yx_records[j + k]); //老描述
                        String newDesc = oldDesc.replaceAll(devCopy_from, devCopy_to.get(i)); //新设备描述替换老设备描述
                        yx_value[k] = newDesc.getBytes();
                    } else if (k == 6) { //更换遥信参数表设备代码字段
                        String newDevCode = getDevCodeBySubAndDevDesc(subCopy_to,devCopy_to.get(i));
                        yx_value[k] = newDevCode.getBytes();
                    } else if (k == 11) { //更换遥信参数表终端序号字段
                        String newTerminalNo = getTerminalNoByDesc(terminalCopy_to);
                        yx_value[k] = newTerminalNo.getBytes();
                    } else if (k == 12){ //更换遥信序号字段
                        String calc = this.subStr(yx_records[j + 35]); //获取计算量标志
                        int newYCNo = 0;
                        if (calc.equals("0")){
                            //非计算量
                            nonCalcRecordCount++;
                            newYCNo = (tableNonCalcRecordCount - 1)  + nonCalcRecordCount;
                        }else if (calc.equals("1")){
                            //计算量
                            calcRecordCount++;
                            int minCalcNo = getMinCalcNo(terminalCopy_to,"遥信参数表");
                            newYCNo = minCalcNo + (tableCalcRecordCount - 1) + calcRecordCount;
                        }
                        yx_value[k] = (newYCNo + "").getBytes();
                    } else {
                        yx_value[k] = yx_records[j + k];
                    }
                }
                //插入一条记录
                this.insertARecord(ConstantDef.MODELDB,"遥信参数表",yx_value);
                int progressValue = 66 + (int)((((float)i)/devCopy_to.size())*33);
                setProgressbarValue(progressValue);
            }
        }
        this.closeTableInfo();
    }

    //执行电度参数复制
    private void executeKWHParaCopy(){
        byte[][] kwh_records = this.getKWHParaInfoBySubAndDevDesc(subCopy_from, devCopy_from);
        //获取电度参数表域的个数
        int kwh_field_count = this.openTableInfo(ConstantDef.MODELDB, "电度参数表");
        //外层循环，按个复制设备
        for (int i = 0; i < devCopy_to.size(); i++){
            //用于记录目标设备的第几条完整的记录。
            int calcRecordCount = 0;
            int nonCalcRecordCount = 0;
            //中层循环，逐条向电度参数表插入记录
            int tableNonCalcRecordCount = getPointTableNonCalcRecordNum(terminalCopy_to,"电度参数表"); //目标终端下挂的测点个数(非计算量)
            int tableCalcRecordCount = getPointTableCalcRecordNum(terminalCopy_to, "电度参数表"); //目标终端下挂的测点个数(计算量)
            for(int j = 0;  j < kwh_records.length;  j += kwh_field_count){
                byte[][] kwh_value = new byte[kwh_field_count][0];
                //内层循环，逐个字段赋值，形成一条完整的记录
                for(int k = 0; k < kwh_field_count; k++) {
                    if (k == 3) { //更换电度参数表代码字段
                        String oldCode = this.subStr(kwh_records[j + k]); //老代码
                        String newDevCode = getDevCodeBySubAndDevDesc(subCopy_to, devCopy_to.get(i)); //新设备代码
                        String oldDevCode = getDevCodeBySubAndDevDesc(subCopy_from, devCopy_from); //老设备代码
                        String targetCode = oldCode.replaceAll(oldDevCode,newDevCode); //将新设备代码替换老设备代码
                        kwh_value[k] = targetCode.getBytes();
                    } else if(k == 4){ //更换电度参数表描述字段
                        String oldDesc = this.subStr(kwh_records[j + k]); //老描述
                        String newDesc = oldDesc.replaceAll(devCopy_from, devCopy_to.get(i)); //新设备描述替换老设备描述
                        kwh_value[k] = newDesc.getBytes();
                    } else if (k == 6) { //更换电度参数表设备代码字段
                        String newDevCode = getDevCodeBySubAndDevDesc(subCopy_to,devCopy_to.get(i));
                        kwh_value[k] = newDevCode.getBytes();
                    } else if (k == 8) { //更换电度参数表终端序号字段
                        String newTerminalNo = getTerminalNoByDesc(terminalCopy_to);
                        kwh_value[k] = newTerminalNo.getBytes();
                    } else if (k == 9){ //更换电度序号字段
                        String calc = this.subStr(kwh_records[j + 26]); //获取计算量标志
                        int newYCNo = 0;
                        if (calc.equals("0")){
                            //非计算量
                            nonCalcRecordCount++;
                            newYCNo = (tableNonCalcRecordCount - 1)  + nonCalcRecordCount;
                        }else if (calc.equals("1")){
                            //计算量
                            calcRecordCount++;
                            int minCalcNo = getMinCalcNo(terminalCopy_to,"电度参数表");
                            newYCNo = minCalcNo + (tableCalcRecordCount - 1) + calcRecordCount;
                        }
                        kwh_value[k] = (newYCNo + "").getBytes();
                    } else {
                        kwh_value[k] = kwh_records[j + k];
                    }
                }
                //插入一条记录
                this.insertARecord(ConstantDef.MODELDB,"电度参数表",kwh_value);
                int progressValue = 99 + (int)((((float)i)/devCopy_to.size())*33);
                setProgressbarValue(progressValue);
            }
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

    //根据厂站和设备描述获取设备代码
    private String getDevCodeBySubAndDevDesc(String sub, String dev){
        int[] fieldcount = new int[1];
        String subCode = getSubCodeByDesc(sub);
        String wh = "描述" + " = '" + dev + "'" + " and 站所线系代码" + " = '" + subCode + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表",
                "代码", wh, "", fieldcount, false);
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

    //获取所有厂站，即所有厂站都可以作为目标厂站
    public ArrayList<String> getTargetSub(){
        ArrayList<String> targetSub = new ArrayList<String>();
        //获取所有厂站信息
        byte[][] subInfo = querySubParaInfo();
        for(int i = 0;i<subInfo.length; i=i+2){
            String str = dlllink.substr(subInfo[i+1]);
            targetSub.add(str);
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

    //根据所选择的厂站，获取其对应的终端列表
    public ArrayList<String> getTerminalListBySub(String sub){
        int[] fieldcount = new int[1];
        String wh = "站所线系代码" + " = '" + getSubCodeByDesc(sub) + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                "描述", wh, "终端序号", fieldcount, false);
        ArrayList<String> ret = new ArrayList<String>();
        for(int i = 0; i < buf.length; i++){
            ret.add(dlllink.substr(buf[i]));
        }
        return ret;
    }

     //根据所选择的厂站，获取厂站下的所有设备
    public ArrayList<String> getDevListBySub(String sub){
        int[] fieldcount = new int[1];
        String wh = "站所线系代码" + " = '" + getSubCodeByDesc(sub) + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表",
                "描述", wh, "DEVID", fieldcount, false);
        ArrayList<String> ret = new ArrayList<String>();
        for(int i = 0; i < buf.length; i++){
            ret.add(dlllink.substr(buf[i]));
        }
        return ret;
    }

    //根据厂站和设备描述获取设备类型
    public String getDevTypeBySubAndDevDesc(String sub, String dev){
        int[] fieldcount = new int[1];
        String wh = "描述" + " = '" + dev + "'" + " and 站所线系代码" + " = '" + getSubCodeByDesc(sub) + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表",
                "类型", wh, "", fieldcount, false);
        return dlllink.substr(buf[0]);
    }

    //根据所选择的厂站和设备类型，获取厂站下的所有该类型的设备
    public ArrayList<String> getDevListBySubAndDev(String sub, String type){
        int[] fieldcount = new int[1];
        String wh = "站所线系代码" + " = '" + getSubCodeByDesc(sub) + "'" + " and 类型" + " = '" + type + "'";
        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "SCD设备参数表",
                "描述", wh, "DEVID", fieldcount, false);
        ArrayList<String> ret = new ArrayList<String>();
        for(int i = 0; i < buf.length; i++){
            ret.add(dlllink.substr(buf[i]));
        }
        return ret;
    }

    //获取厂站下所有的非空设备
    public ArrayList<String> getNotEmptyDev(String sub){
        ArrayList<String> ret = new ArrayList<String>();
        ArrayList<String> devList  = getDevListBySub(sub);
        for (int i = 0; i < devList.size(); i++){
            if (!ifEmptyDev(sub,devList.get(i))){
                ret.add(devList.get(i));
            }
        }
        return ret;
    }

    //根据所选择的厂站和设备，获取该厂站下所有该类型的空设备
    public ArrayList<String> getEmptyDevOfSameType(String sub, String type){
        ArrayList<String> ret = new ArrayList<String>();
        ArrayList<String> devList  = getDevListBySubAndDev(sub,type);
        for (int i = 0; i < devList.size(); i++){
            if (ifEmptyDev(sub,devList.get(i))){
                ret.add(devList.get(i));
            }
        }
        return ret;
    }

    //根据厂站和设备描述判断该设备是否为空设备
    public boolean ifEmptyDev(String sub, String dev){
        boolean ret = false;
        //遥信参数表、遥测参数表、电度参数表都为空，那么认为该设备为空设备
        if (getYXParaInfoBySubAndDevDesc(sub,dev).length == 0
                && getYCParaInfoBySubAndDevDesc(sub,dev).length == 0
                && getKWHParaInfoBySubAndDevDesc(sub,dev).length == 0){
            ret = true;
        }
        return ret;
    }

    //检索遥测参数表，获取所有指定设备代码的遥测点
    public byte[][] getYCParaInfoBySubAndDevDesc(String sub,String dev){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "设备代码" + " = '" + getDevCodeBySubAndDevDesc(sub, dev) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥测参数表",
                "*", wh, "遥测序号", fieldcount, false);
        return buf;
    }

    //检索遥信参数表，获取所有指定设备代码的遥信点
    public byte[][] getYXParaInfoBySubAndDevDesc(String sub,String dev){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "设备代码" + " = '" + getDevCodeBySubAndDevDesc(sub, dev) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥信参数表",
                "*", wh, "遥信序号", fieldcount, false);
        return buf;
    }

    //检索电度参数表，获取所有指定设备代码的电度
    public byte[][] getKWHParaInfoBySubAndDevDesc(String sub,String dev){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "设备代码" + " = '" + getDevCodeBySubAndDevDesc(sub, dev) + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "电度参数表",
                "*", wh, "电度序号", fieldcount, false);
        return buf;
    }

    //检索遥测参数表，获取所有指定设备代码的遥测点(带计算量标志)
    public byte[][] getYCParaInfoBySubAndDevDesc(String sub,String dev,String calc){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "设备代码" + " = '" + getDevCodeBySubAndDevDesc(sub, dev) + "'" + " and 计算量标志" + " = '" + calc + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥测参数表",
                "*", wh, "遥测序号", fieldcount, false);
        return buf;
    }

    //检索遥信参数表，获取所有指定设备代码的遥信点(带计算量标志)
    public byte[][] getYXParaInfoBySubAndDevDesc(String sub,String dev, String calc){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "设备代码" + " = '" + getDevCodeBySubAndDevDesc(sub, dev) + "'" + " and 计算量标志" + " = '" + calc + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "遥信参数表",
                "*", wh, "遥信序号", fieldcount, false);
        return buf;
    }

    //检索电度参数表，获取所有指定设备代码的电度(带计算量标志)
    public byte[][] getKWHParaInfoBySubAndDevDesc(String sub,String dev, String calc){
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "设备代码" + " = '" + getDevCodeBySubAndDevDesc(sub, dev) + "'" + " and 计算量标志" + " = '" + calc + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, "电度参数表",
                "*", wh, "电度序号", fieldcount, false);
        return buf;
    }

    //获取指定终端序号的点表非计算量记录条数
    private int getPointTableNonCalcRecordNum(String terminal, String table){

        //第一步，获取点表域的个数
        int field_count = this.openTableInfo(ConstantDef.MODELDB, table);

        //第二步，查询点表
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "终端序号" + " = '" + getTerminalNoByDesc(terminal) + "'"+ " and 计算量标志" + " = '" + 0 + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, table,
                "*", wh, "", fieldcount, false);

        //第三步，返回记录条数
        return buf.length == 0 ? 0 : buf.length/field_count;
    }

    //获取指定终端序号的点表计算量的记录条数
    private int getPointTableCalcRecordNum(String terminal, String table){

        //第一步，获取点表域的个数
        int field_count = this.openTableInfo(ConstantDef.MODELDB, table);

        //第二步，查询点表
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh = "终端序号" + " = '" + getTerminalNoByDesc(terminal) + "'" + " and 计算量标志" + " = '" + 1 + "'";
        buf = dlllink.queryAnyTable(ConstantDef.MODELDB, table,
                "*", wh, "", fieldcount, false);

        //第三步，返回记录条数
        return buf.length == 0 ? 0 : buf.length/field_count;
    }

    //获取指定终端序号的点表中计算量记录的最小序号数
    private int getMinCalcNo(String terminal, String table){

        String wh = "终端序号" + " = '" + getTerminalNoByDesc(terminal) + "'"+ " and 计算量标志" + " = '" + 1 + "'";

        String noField;
        if (table.equals("遥测参数表")){
            noField = "遥测序号";
        }else if(table.equals("遥信参数表")){
            noField = "遥信序号";
        }else {
            noField = "电度序号";
        }

        byte[][] buf = dlllink.queryAnyTable(ConstantDef.MODELDB, table,
                noField, wh, noField, new int[1], false);

        return buf.length == 0 ? 0 : Integer.parseInt(dlllink.substr(buf[0]));
    }

    //随机五位码发生器
    private String randomStringGenerator(){
        char letter[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
                'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                'u', 'v', 'w', 'x', 'y', 'z'};
        char[] elem = new char[5];
        for(int i = 0; i < elem.length; i++ ){
            elem[i] = letter[(int) (Math.random() * letter.length)];
        }
        return String.valueOf(elem);
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
