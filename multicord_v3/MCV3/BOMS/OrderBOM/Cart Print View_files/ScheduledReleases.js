var viewModelArray=[];var originalViewModel="";var cntr=0;ko.validation.rules.salesMult={validator:function(c,a){var b=parseInt(c);return(a==1||b%a==0)},message:resources.SchMultiple};ko.validation.rules.minQty={validator:function(a){return(a>0)},message:resources.SchQtyGtZero};ko.validation.rules.futureDate={validator:function(a,e){var d=$.datepicker.regional[""];var b=$.datepicker.parseDate(d.dateFormat,e,d);var c=$.datepicker.parseDate(d.dateFormat,a,d);return c>b},message:resources.SchFutureDate};ko.validation.rules.oneYear={validator:function(a){var c=$.datepicker.regional[""];
var b=$.datepicker.parseDate(c.dateFormat,a,c);return b<=new Date(GetDateOneYearFromNow())},message:resources.SchDateOneYear};ko.validation.rules.holiday={validator:function(a){var c=$.datepicker.regional[""];var b=$.datepicker.parseDate(c.dateFormat,a,c);var d=jQuery.datepicker.formatDate("mm-dd",b);return jQuery.inArray(d,GetHolidays())==-1},message:resources.SchDateHoliday};ko.validation.registerExtenders();ko.validation.rules.unique.message=resources.SchDateDuplicate;var viewModel=function(c){var a=this;
a.guid=c.guid;a.id=c.id;a.orderQty=c.orderQty;a.salesMultipleQty=c.salesMultipleQty;a.mouserPartNumber=c.mouserPartNumber;a.schLine=ko.observableArray();a.encryptedId=c.encryptedId;a.hasSchError=c.hasSchError;a.hasProductError=c.hasProductError;a.lineExtPrice=c.lineExtPrice;a.currentDate=c.currentDate;a.errors=ko.validation.group(a,{deep:true,observable:false});a.HasExtPriceError=function(){return(a.schLine().length>=1&&a.lineExtPrice<=schOrderLineMin)};a.TotalSchReleaseQty=ko.computed(function(){var f=0;var d=a.schLine();
for(var e=0;e<d.length;e++){if(d[e].value()){f+=parseInt(d[e].value())}}return f},this,{deferEvaluation:true});a.RemainingCount=ko.computed(function(){var d=a.schLine();if(d.length==0){return a.orderQty}var e=a.TotalSchReleaseQty();return(e>0?(a.orderQty-e):a.orderQty)},this,{deferEvaluation:true});a.AddLine=function(d){if(d){a.schLine.push(new schLineModel({Key:"",Value:""},a));return}$("#pSave_"+a.id).hide();if(a.schLine().length>11){var e=$("#pError_"+a.id);e.html(resources.SchMaxDeliveries);e.show();return
}if(HasErrors(a,true)){return}if(a.HasExtPriceError()&&(mode=="Cart"||mode=="Project")){var e=$("#pError_"+a.id);e.html(resources.SchOrdersLineExtPriceError);e.show();return}if(mode=="Cart"||mode=="OrderHistory"){AddGaTags("Add Shipment","null",a.schLine().length)}a.schLine.push(new schLineModel({Key:"",Value:""},a));AddDatePicker()};a.RemoveLine=function(d){HideMessages(a.id);a.schLine.remove(d);AddDatePicker();ShowPending(a.id);if(mode=="Cart"||mode=="OrderHistory"){AddGaTags("Shipment Removed","Shipment Delete",a.schLine().length)
}};if(c.schLine){for(var b=0;b<c.schLine.length;b++){a.schLine.push(new schLineModel(c.schLine[b],a));if(mode=="OrderHistory"){a.originalSchTotal=a.TotalSchReleaseQty()}}}a.RemainingAsapCount=ko.computed(function(){var e=a.schLine();if(e.length==0){return a.originalSchTotal}var f=a.TotalSchReleaseQty();var d=a.originalSchTotal-f;return(d>0?d:0)},this,{deferEvaluation:true});a.schLine.subscribe(function(d){if(d.length>=1&&d[0].key().length>0){ShowPending(c.id)}});a.IsBound=false};var schLineModel=function(c,b){var a=this;
a.key=ko.observable(c.Key=="01/01/1900"?"":c.Key);a.value=ko.observable(c.Value+"");if(mode=="Cart"||mode=="OrderHistory"||mode=="Project"){a.value.extend({number:true,required:{message:resources.SchQtyRequired},minQty:true,max:{params:b.orderQty,message:resources.SchQtyGtOrderQty},salesMult:b.salesMultipleQty});a.key.extend({date:true,required:{message:resources.SchDateRequired},futureDate:b.currentDate,unique:{collection:b.schLine,valueAccessor:function(d){return(d.Key!=null?d.key:d.key())}},oneYear:true,holiday:true})
}a.value.subscribe(function(d){if(d.length>0&&b.isValid()){ShowPending(b.id)}else{HideMessages(b.id);b.errors.showAllMessages(true)}});a.key.subscribe(function(d){if(d.length>0&&b.isValid()){ShowPending(b.id)}else{HideMessages(b.id);b.errors.showAllMessages(true)}})};$(document).ready(function(){Init()});function Init(){var c=false;for(var b=0;b<jsonObj.length;b++){for(var a=0;a<jsonObj[b].schLine.length;a++){if(jsonObj[b].schLine[a].Value>0){c=true}else{jsonObj[b].schLine[a].Value=""}}}if(!c){$("a.sch-order").show()
}if(c){AddClickEvent();ShowScheduledLines(true);$("#removeSchReleases").show();if(mode=="Unspecified"){$("#schReadOnly").show()}else{if(mode=="ReadOnly"){$("tr[id$=trSchShipments]").after('<tr><td colspan="2"><a href="javascript:void(\'0\');" onclick="ShowShipmentTotals();">'+resources.ViewShipmentTotals+"</a></td><tr><td>&nbsp;</td></tr>")}else{if(mode=="OrderHistory"){$("input#btnSaveAll,input#btnSaveAllBottom").show()}}}if(mode=="ReadOnly"||mode=="CartPrintView"){$("tr.green-bg").each(function(){$(this).find("td:first").addClass("hidden")
})}else{$("tr.green-bg").each(function(){if(mode=="OrderHistory"||mode=="Unspecified"){$(this).find("td:first").attr("colspan",4)}else{if(mode=="OrderHistoryPrintView"){$(this).find("td:first").attr("colspan",3)}}})}}}function AddGaTags(c,a,b){try{_gaq.push(["_trackEvent",(mode=="OrderHistory"?"Schedule Order Update":"Schedule Order"),c,a,b])}catch(d){}}function AddGaTagsNoValue(b,a){AddGaTags(b,a,0)}function ShowPending(a){if(mode=="OrderHistory"||mode=="Cart"||mode=="Project"){if(mode=="OrderHistory"){window.onbeforeunload=function(){return resources.SchUnsavedChanges
}}else{$("#btnSchSave_"+a).show()}$("input#btnSaveAll,input#btnSaveAllBottom").removeClass("disabled");UnbindUpdateEvent();$("input#btnSaveAll,input#btnSaveAllBottom,input[id$=btn5],input[id$=btn7],input[id$=btn9],input[id$=btn10],input[id$=btn15],input[id$=btnUpdateTop],input[id$=btnOrderProjectTop],input[id$=btnUpdateProject],input[id$=btnOrderProject]").click(function(){ShowSaveChangesModal($(this));return false})}if(mode!="OrderHistory"){$("#pPending_"+a).show()}$("#pSave_"+a).hide()}function HideMessages(a){if(mode=="OrderHistory"){$("input#btnSaveAll,input#btnSaveAllBottom").addClass("disabled");
$("input#btnSaveAll,input#btnSaveAllBottom").unbind("click");window.onbeforeunload=null}$("#btnSchSave_"+a).hide();$("#pSave_"+a).hide();$("#pPending_"+a).hide()}function Save(c){if(HasErrors(c,false)){HideMessages(c.id);return}HideMessages(c.id);var a=ko.toJSON(c);var b=ko.toJSON(originalViewModel[c.id-1]);if(a===b){return}$.ajax({url:"/api/ScheduleReleases/Insert?guid="+encodeURIComponent(c.guid)+"&encryptedId="+encodeURIComponent(c.encryptedId)+"&mode="+encodeURIComponent(mode),cache:false,timeout:20000,contentType:"application/json",type:"POST",data:a,success:function(){$("#pSave_"+c.id).show();
if(c.TotalSchReleaseQty()>0){$("#removeSchReleases").show();$("#schPendingDisclaimer").show();originalViewModel[c.id-1]=JSON.parse(a);$("a.sch-order").hide();UnbindUpdateEvent();AddClickEvent();AddGaTags("Changes Saved","null",c.schLine().length)}else{$("#removeSchReleases").hide()}if(c.hasSchError){$("input[id$=btn7]").click()}},error:function(d){$("#btnSchSave_"+c.id).show();var e=$("#pError_"+c.id);e.html(resources.SchSaveError);e.show()}})}function DeleteAllSchReleases(){if(!viewModelArray||viewModelArray.length==0){return
}var e=[];var a=false;for(var c=0;c<viewModelArray.length;c++){var b=viewModelArray[c];if(b.hasSchError){a=true}e.push(b.encryptedId);HideMessages(b.id)}var d=JSON.stringify(e);$.ajax({url:"/api/ScheduleReleases/DeleteAll?guid="+encodeURIComponent(viewModelArray[0].guid)+"&mode="+encodeURIComponent(mode),cache:false,timeout:20000,contentType:"application/json",type:"POST",data:d,success:function(){if(a){$("input[id$=btn7]").click()}else{ResetScheduledLines()}},error:function(){$("#removeSchReleasesError").show()
}})}function SaveAll(g,a){if(!viewModelArray||viewModelArray.length==0){return}var e=[];var b=false;for(var d=0;d<viewModelArray.length;d++){var j=viewModelArray[d];var f=ko.toJSON(j);var h=ko.toJSON(originalViewModel[d]);if(f!==h){if(HasErrors(j,false)){HideMessages(j.id);b=true}e.push(j)}}if(b){$("button#btnConfirm").hide();$("div#saveChangesModal p").html(resources.SchOrderErrors);$("button#btnCancel").text(resources.lnkClose);return}var c=ko.toJSON(e);$.ajax({url:"/api/ScheduleReleases/SaveAll?salesOrderNumber="+encodeURIComponent(salesOrderNumber)+"&customerUvid="+encodeURIComponent(customerUvid)+"&mode="+encodeURIComponent(mode),cache:false,timeout:20000,contentType:"application/json",type:"POST",data:c,success:function(){g.dialog("destroy");
if(mode=="OrderHistory"){window.onbeforeunload=null;window.location.href=$("a[id$=lnkViewOrderHistory]").attr("href")}else{UnbindUpdateEvent();a.click()}},error:function(){cntr++;$("button#btnCancel").text(resources.btnCancel);if(cntr>=3){cntr=0;$("button#btnConfirm").hide();$("div#saveChangesModal p").html(resources.SchSaveAllError)}else{$("button#btnConfirm").text(resources.btnESContinue);$("div#saveChangesModal p").html(resources.SchSaveAllErrorContinue)}}})}function ShowSaveChangesModal(b){var a=(mode=="OrderHistory"?resources.SchSaveAll:resources.SaveChangesToSch);
var c=$('<div id="saveChangesModal" class="confirmLicenseDialog"><p>'+a+"</p></div>");c.dialog({title:resources.btnSaveFreightAcctChange,resizable:false,modal:true,width:440,height:200,maxHeight:550,close:function(){if(mode=="Cart"||mode=="OrderHistory"){AddGaTagsNoValue("Save Changes Modal","Closed")}},buttons:[{id:"btnConfirm",text:(mode=="OrderHistory"?resources.confirm:resources.btnSave),"class":"ui-button",click:function(){if(mode=="Cart"||mode=="OrderHistory"){AddGaTagsNoValue("Save Changes Modal",(mode=="Cart"?"Save":"Confirm"))
}if(mode=="Cart"){AddSchEventTracking()}SaveAll($(this),b)}},{id:"btnCancel",text:(mode=="OrderHistory"?resources.btnCancel:resources.DontSave),click:function(){if(mode=="Cart"||mode=="OrderHistory"){AddGaTagsNoValue("Save Changes Modal",(mode=="Cart"?"Don't Save":"Cancel"))}if(($("button#btnCancel").text()==resources.lnkClose||mode=="OrderHistory"||$("button#btnCancel").text()==resources.btnCancel)&&$.scrollTo&&$("p.error:visible:first").length>0){$(this).dialog("destroy");if(b.data("position")=="top"){$.scrollTo($("p.error:visible:first").closest("td"))
}else{$.scrollTo($("p.error:visible:last").closest("td"))}}else{if(mode!="OrderHistory"&&$("button#btnCancel").text()==resources.DontSave){$(this).dialog("destroy");UnbindUpdateEvent();b.click()}else{$(this).dialog("destroy")}}}}]})}function ShowShipmentTotals(){var a=$('<div id="schShipTotals" class="confirmLicenseDialog">'+$("#divSchShipTotals").html()+"</div>");a.dialog({title:resources.SchShipmentTotals,resizable:false,modal:true,width:440,height:200,maxHeight:550,buttons:[{id:"btnCancel",text:resources.btnCancel,click:function(){$(this).dialog("destroy")
}}]})}function AddClickEvent(){$("div[id$=pnl2] input.btn-Checkout").unbind();$("div[id$=pnl2] input.btn-Checkout").click(function(){AddSchEventTracking()})}function ResetScheduledLines(){$("a.sch-order").show();$("#removeSchReleases").hide();$("#removeSchReleasesError").hide();$("tr.green-bg").hide();var c=0;for(var d=0;d<viewModelArray.length;d++){var a=viewModelArray[d];if(a.TotalSchReleaseQty()>0){var b=$('tr[data-partnumber="'+a.mouserPartNumber+'"]:nth(1)');b.find("span.sch-remaining").text(a.orderQty);
var e=b.find("div#divSchItems div.padding5");e.find("input").val("");if(e.length>1){b.find("div#divSchItems div.padding5:not(:first)").remove()}}a.schLine.removeAll();c+=a.orderQty;a.AddLine(true)}originalViewModel=JSON.parse(ko.toJSON(viewModelArray));if(viewModelArray.length==0){return}AddGaTags("Scheduler Removed",viewModelArray[0].guid,c);AddGaTags("Shipment Removed","Switch to regular Order",0)}function AddSchEventTracking(){if(viewModelArray.length==0){return}var d=0;var a=0;for(var e=0;e<viewModelArray.length;
e++){var b=viewModelArray[e];d+=b.TotalSchReleaseQty();if(b.schLine()&&b.TotalSchReleaseQty()>0){for(var c=0;c<b.schLine().length;c++){if(b.schLine()[c]){a++}}}}AddGaTags("Scheduled Order Checkout",viewModelArray[0].guid,d);AddGaTags("Scheduled Ship Dates","With Scheduled Orders",a)}function UnbindUpdateEvent(){$("input[id$=btn5],input[id$=btn7],input[id$=btn9],input[id$=btn10],input[id$=btn15],input[id$=btnUpdateTop],input[id$=btnOrderProjectTop],input[id$=btnUpdateProject],input[id$=btnOrderProject]").unbind("click")
}function HasErrors(c,a){if(!c.isValid()){c.errors.showAllMessages(true);return true}var b=c.TotalSchReleaseQty();var d=$("#pError_"+c.id);if(b>c.orderQty){d.html(resources.OrderQtyGtOrEqualSchQty);d.show();HideMessages(c.id);return true}if(!a&&mode=="OrderHistory"&&(b+c.RemainingAsapCount())>c.originalSchTotal){d.html(String.format(resources.OrderQtyEqualQtySch,c.originalSchTotal));d.show();HideMessages(c.id);return true}d.hide();return false}function ShowScheduledLines(b){if(hasErrors=="False"||b){$("tr.green-bg").show();
$("a.sch-order").hide();BindScheduledLines();if(viewModelArray.length==0){return}if(mode=="Cart"&&$.scrollTo){$.scrollTo("table[id$=grid]")}if(mode=="Cart"&&!b){AddGaTags("Scheduler Opened",viewModelArray[0].guid,TotalOrderQty());AddGaTagsNoValue("Scheduled Order Button","No Pop-up Modal")}}else{if(mode=="Cart"||mode=="Project"){if(mode=="Cart"){AddGaTagsNoValue("Scheduled Order Button","Pop-up Modal")}var a=resources.SchOrdersRulesError;var c=$('<div id="schOnline" class="confirmLicenseDialog"><p>'+a+"</p></div>");
c.dialog({title:resources.SchOrdersOnline,resizable:false,modal:true,width:560,height:301,maxHeight:550,buttons:[{id:"btnConfirm",text:resources.lnkClose,click:function(){$(this).dialog("destroy")}}]})}}}function TotalOrderQty(){var b=0;for(var c=0;c<viewModelArray.length;c++){var a=viewModelArray[c];b+=a.orderQty}return b}function FindViewModelByPartNumber(a){var c="";for(var d=0;d<viewModelArray.length;d++){var b=viewModelArray[d];if(b.mouserPartNumber==a){c=b;break}}return c}function BindScheduledLine(a,f,d){var c=FindViewModelByPartNumber(a);
if(c&&!c.isBound){var e=$('tr[data-partnumber="'+a+'"]');if(e.length>0){c.isBound=true;ko.applyBindingsWithValidation(c,e[1]);AddDatePicker();var g=e.find("input.sch-qty-input");g.unbind("onfocus");e.find("a.add-line").attr("onclick",null);switch(f){case"date":if(e.find("input.sch-date-input:nth("+d+")")){e.find("input.sch-date-input:nth("+d+")").datepicker("show")}break;case"addline":c.AddLine(false);break;case"removeline":if(c.schLine()[d]){var b=c.schLine()[d];c.RemoveLine(b)}break;default:if(g[d]){g[d].focus()
}break}}}}function BindScheduledLines(){if(viewModelArray.length==0){for(var d=0;d<jsonObj.length;d++){var c=new viewModel(jsonObj[d]);viewModelArray.push(c)}originalViewModel=JSON.parse(ko.toJSON(viewModelArray))}ko.validation.configure({insertMessages:false,decorateElement:true,errorElementClass:"error",messagesOnModified:false});var f=$("table[id$=grid]");var g=$("#tblSchItems");if($("#tblSchItems").find("tr").length>0){for(var e=0;e<viewModelArray.length;e++){var a=viewModelArray[e];var b=g.find('tr[data-partnumber="'+a.mouserPartNumber+'"]').detach();
f.find('tr[data-partnumber="'+a.mouserPartNumber+'"]').after(b)}}AddDatePicker()}function GetHolidays(){return[]}function GetDateOneYearFromNow(){var e=new Date();var f=e.getFullYear();var a=e.getDate();var b=(f+1)+"-"+("0"+(e.getMonth()+1)).slice(-2)+"-"+("0"+a).slice(-2);var c=new String(b).split("-");return new Date(c[0],c[1]-1,c[2])}function GetNextDay(){var a=new Date();var b=new Date(a.getFullYear(),a.getMonth(),a.getDate()+1);return("0"+(b.getMonth()+1)).slice(-2)+"-"+("0"+b.getDate()).slice(-2)}function AddDatePicker(){var a=GetHolidays();
if(mode=="OrderHistory"){a.push(GetNextDay())}try{$(".datepicker").removeClass("hasDatepicker");$(".datepicker").datepicker({showOtherMonths:true,selectOtherMonths:true,minDate:1,maxDate:GetDateOneYearFromNow(),constrainInputType:true,showButtonPanel:true,beforeShowDay:function(d){var e=$.datepicker.noWeekends(d);var c=jQuery.datepicker.formatDate("mm-dd",d);if(e[0]){return[a.indexOf(c)==-1]}else{return e}},beforeShow:function(c){var d=$("input#"+c.id).parents("div#divSchItems");var e=d.find(".datepicker").length;
if(e>1){d.find(".datepicker:nth("+(e-1)+")").datepicker("option","defaultDate",d.find(".datepicker:nth("+(e-2)+")").datepicker("getDate"))}setTimeout(function(){var f=$(c).datepicker("widget").find(".ui-datepicker-close");f.unbind("click").bind("click",function(){$.datepicker._clearDate(c);if(mode=="OrderHistory"){AddGaTagsNoValue("Date Clicked","Clear")}})},1)},onClose:function(d,c){if(mode=="OrderHistory"){if(d==c.lastVal){AddGaTagsNoValue("Date Clicked","No Date Selected")}else{if(d.length>0){AddGaTagsNoValue("Date Clicked","Date Selected")
}}}},closeText:resources.Clear,buttonText:""})}catch(b){}};