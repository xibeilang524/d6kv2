package com.daqo.dbinfo.table;

import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//pdb_field表信息
public class PDB_FieldTable {

  public int pdb_field_describle_no; //describe在pdb_field中的列号
  public int pdb_field_tablename_no; //tablename在pdb_field中的列号
  public int pdb_field_dispstyle_no; //displaystyle在pdb_field中的列号
  public int pdb_field_dotnum_no; //dotnum在pdb_field中的列号
  public int pdb_field_evaluemode_no; //evaluatemode在pdb_field中的列号
  public int pdb_field_datasource_no; //datasource在pdb_field中的列号
  public int pdb_field_srcdatafield_no; //srcdatafield在pdb_field中的列号
  public int pdb_field_searchedfield_no; //searchedfield在pdb_field中的列号
  public int pdb_field_conditionflag_no; //conditionflag在pdb_field中的列号
  public int pdb_field_conditionfield_no; //conditionfield在pdb_field中的列号
  public int pdb_field_valuefrom_no; //valuefrom在pdb_field中的列号
  public int pdb_field_valueto_no; //valueto在pdb_field中的列号
  public int pdb_field_relatefield_no; //relatefield在pdb_field中的列号
  public int pdb_field_relvatefrom_no; //relvaluefrom在pdb_field中的列号
  public int pdb_field_relvalueto_no; //relvalueto在pdb_field中的列号
  public int pdb_field_calcfield1_no; //calcfield1在pdb_field中的列号
  public int pdb_field_calcfield2_no; //calcfield2在pdb_field中的列号
  public int pdb_field_calcsymbol_no; //calcsymbole在pdb_field中的列号
  public int pdb_field_status_no; //status在pdb_field中的列号
  public int pdb_field_tabno_no; //tabno在pdb_field中的列号
  public int pdb_field_defaultvalue_no; //defaultvalue在pdb_field中的列号
  public int pdb_field_fieldprop_no; //fieldprop在pdb_field中的列号
  public int pdb_filed_alias_no;     //alias在pdb_field中的序号
  FieldProp pdbf_field;
  public int fieldnums;
  public PDB_FieldTable() {
    pdbf_field = new FieldProp(ConstantDef.MODELDB,
                               ConstantDef.PARA_FIELD_DESCRIBLE);
    pdbf_field.getTableInfo(); //得到pdb_field表信息
    fieldnums = pdbf_field.fieldnums;
    pdb_field_describle_no = pdbf_field.getColumnNo(ConstantDef.PDBF_DESCRIBE); //describe在pdb_field中的列号
    pdb_field_tablename_no = pdbf_field.getColumnNo(ConstantDef.PDBF_TABLENAME); //tablename在pdb_field中的列号
    pdb_field_dispstyle_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_DISPLAYSTYLE); //displaystyle在pdb_field中的列号
    pdb_field_dotnum_no = pdbf_field.getColumnNo(ConstantDef.PDBF_DOTNUM); //dotnum在pdb_field中的列号
    pdb_field_evaluemode_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_EVALUATEMODE); //evaluatemode在pdb_field中的列号
    pdb_field_datasource_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_DATASOURCE); //datasource在pdb_field中的列号
    pdb_field_srcdatafield_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_SRCDATAFIELD); //srcdatafield在pdb_field中的列号
    pdb_field_searchedfield_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_SEARCHEDFIELD); //searchedfield在pdb_field中的列号
    pdb_field_conditionflag_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_CONDITIONFLAG); //conditionflag在pdb_field中的列号
    pdb_field_conditionfield_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_CONDITIONFIELD); //conditionfield在pdb_field中的列号
    pdb_field_valuefrom_no = pdbf_field.getColumnNo(ConstantDef.PDBF_VALUEFROM); //valuefrom在pdb_field中的列号
    pdb_field_valueto_no = pdbf_field.getColumnNo(ConstantDef.PDBF_VALUETO); //valueto在pdb_field中的列号
    pdb_field_relatefield_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_RELATEFIELD); //relatefield在pdb_field中的列号
    pdb_field_relvatefrom_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_RELVALUEFROM); //relvaluefrom在pdb_field中的列号
    pdb_field_relvalueto_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_RELVALUETO); //relvalueto在pdb_field中的列号
    pdb_field_calcfield1_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_CALCFIELD1); //calcfield1在pdb_field中的列号
    pdb_field_calcfield2_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_CALCFIELD2); //calcfield2在pdb_field中的列号
    pdb_field_calcsymbol_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_CALCSYMBOLE); //calcsymbole在pdb_field中的列号
    pdb_field_status_no = pdbf_field.getColumnNo(ConstantDef.PDBF_STATUS); //status在pdb_field中的列号
    pdb_field_tabno_no = pdbf_field.getColumnNo(ConstantDef.PDBF_TABNO); //tabno在pdb_field中的列号
    pdb_field_defaultvalue_no = pdbf_field.getColumnNo(ConstantDef.
        PDBF_DEFAULTVALUE); //defaultvalue在pdb_field中的列号
    pdb_field_fieldprop_no = pdbf_field.getColumnNo(ConstantDef.PDBF_FIELDPROP); //fieldprop在pdb_field中的列号
    if(PreProcessor.ALIAS)
    {
       pdb_filed_alias_no=pdbf_field.getColumnNo(ConstantDef.PDBF_ALIAS);
    }
  }
}
