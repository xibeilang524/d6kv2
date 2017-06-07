create or replace view subareaterminalview
(终端序号, 终端代码, 终端描述, 站所线系代码, 站所线系描述, id, name, masterid)
as
select 终端参数表.终端序号,
终端参数表.代码,
终端参数表.描述,
站所线系参数表.代码,
站所线系参数表.描述,
stationarea.id,
stationarea.NAME,
stationarea.MASTERID
from 站所线系参数表,终端参数表,stationarea
where 终端参数表.站所线系代码 = 站所线系参数表.代码 and (站所线系参数表.区域序号 = stationarea.id or 站所线系参数表.区域序号 = 0)
order by stationarea.id;