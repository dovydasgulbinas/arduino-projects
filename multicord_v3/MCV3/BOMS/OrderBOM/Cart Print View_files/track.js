$(document).ready(function(){var a=undefined;$("a[href^='http://']:not([href$=pdf]):not([id$='lnkDataSheet'])").click(function(){if($(this).attr("target")=="_blank"){a="/outgoing/"+jQuery.url.segment(0)+"/"+$(this).attr("href").replace("http://","");if(a!=undefined){trackLinks(a)}}});$("a[href^='http://'][href$=pdf]:not([id$='lnkDataSheet'])").click(function(){if($(this).attr("target")=="_blank"){a="/outgoing/pdf/"+$(this).attr("href").replace("http://","").replace("../","");if(a!=undefined){trackLinks(a)}}});
$("a[href$=pdf]:not([href^='http://']):not([id$='lnkDataSheet'])").click(function(){if($(this).attr("target")=="_blank"){a="/download/pdf/"+$(this).attr("href").replace("http://","").replace("../","");if(a!=undefined){trackLinks(a)}}});$("a[id$='lnkDataSheet']").click(function(){if($(this).attr("target")=="_blank"){a="/outgoing/datasheets/"+$(this).parents("tr").find("a[id$='lnkMouserPartNumber']").html()}else{a="/download/datasheets/"+$(this).parents("tr").find("a[id$='lnkMouserPartNumber']").html()}if(a!=undefined){trackLinks(a)
}});$(".tooltip").tooltip({track:true,delay:0,showURL:false,showBody:" - ",fade:250});$("div[id^='popup'] a").click(function(){a="/popup/"+$(this).parents("tr").find("a[id$='lnkMouserPartNumber']").html();if(a!=undefined){trackLinks(a)}});if($("div[id*='FindSimilarPnl']").length>0){findsimilar("init");if($("input[id*='btnfindsimilar']").length>0){if($("input[id*='btnfindsimilar']").val()=="Find Similar"){$("input[id*='btnfindsimilar']").attr("value","Show Similar")}}}$("td[class^='find-similar'] input").click(findsimilar);
if($("span[id$='ContentMain_lblNoResults']").text()=="NO RESULTS FOUND."){$("span[id$='ContentMain_lblNoResults']").html("An unexpected error has occurred.<br/>We are sorry for the inconvenience.")}$("a[href*='-%E2%80%93-']").each(function(){$(this).attr("href",$("a[href*='-%E2%80%93-']").attr("href").replace("-%E2%80%93-","-"))});var b=window.location.toString();if(b.indexOf("-%e2%80%93-")>0||b.indexOf("-%E2%80%93-")>0){window.location=b.replace("-%e2%80%93-","-").replace("-%E2%80%93-","-")}});function trackLinks(b){try{pageTracker._trackPageview(b)
}catch(a){}}function findsimilar(a){var b="";var c=0;$("td[class^='find-similar'] input").each(function(){if($(this).is(":checked")){var d=$(this).attr("id").split("_");b=b+d[d.length-1]+"+";c++}else{if(a=="init"){if(c==2){$(this).prop("checked",true);var d=$(this).attr("id").split("_");b=b+d[d.length-1]+"+"}a="ready"}}});if(b==""){$("#lblfindsimilarcount").remove();$("div[id*='divfindsimilarhelp']").remove();$("input[id*='btnfindsimilar']").after("<div id='divfindsimilarhelp'  style='text-align:left'><img border='0' id = 'imghelp' src='http://www.mouser.com/Images/help-icon.png' ></img>&nbsp;To show similar products, select at least one checkbox and click the show similar button.</div>");
return}else{if($("div[id$='divselectiontxt']").text()==""){$("div[id*='divfindsimilarhelp']").html("<div id='divselectiontxt' style='float:right;text-align:right;padding-top:5px;'>Per above selection(s)</div><img border='0' id = 'imghelp' src='http://www.mouser.com/Images/help-icon.png' ></img>&nbsp;Show stocked product(s), check and uncheck the boxes above.")}}b=b+"1323038+1323043";if($("#divfindsimilarhelp").length==0){$("input[id*='btnfindsimilar']").after("<div id='divfindsimilarhelp'  style='text-align:left'><div id='divselectiontxt' style='float:right;text-align:right;padding-top:5px;'>Per above selection(s)</div><img border='0' id = 'imghelp' src='http://www.mouser.com/Images/help-icon.png' ></img>&nbsp;Show stocked product(s), check and uncheck the boxes above.</div>")
}$("input[id*='btnfindsimilar']").before("<img border='0' style='vertical-align:middle' id = 'imgspinner' src='http://www.mouser.com/Images/spinner.gif' />");$.ajax({url:"/ajax/parametricoutput.ashx",type:"POST",data:"N="+b,success:function(d){$("#lblfindsimilarcount").remove();d=d.split(" ")[0];if(d!="0"){d=d-1}if(d=="0"&&a=="ready"){if($("td[class^='find-similar'] input:first").is(":checked")){$("td[class^='find-similar'] input:first").prop("checked",false);findsimilar("ready")}else{if($("td[class^='find-similar'] input:eq(2)").is(":checked")){$("td[class^='find-similar'] input:eq(2)").prop("checked",false);
findsimilar("ready")}}}if(d=="0"){$("div[id$='divselectiontxt']").text("Refine above selection(s)")}else{$("div[id$='divselectiontxt']").text("Per above selection(s)")}$("input[id*='btnfindsimilar']").after("<div id='lblfindsimilarcount' style='text-align:left'><div style='float:right;text-align:right;padding-top:0px;'><span class='red' style='text-align:right'>Found: "+d+"</span></div><div class='red' style=''>Show Stocked Products With Similar Attributes.</div></div>");$("span[id$='lblhiddenfindsimilarcount']").val(d);
$("#imgspinner").remove()},error:function(d){$("#imgspinner").remove()}})};