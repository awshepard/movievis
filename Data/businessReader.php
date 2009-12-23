<?php

//$PATH = "C:\Users\Adam\Documents";
$PATH = "C:\Documents and Settings\ashepard\My Documents";

$handle = fopen("$PATH\business.list\business.list","r");
$handle_write =fopen("$PATH\business.list\business.massaged","w");

var_dump($handle);

while (!feof($handle))
{
	$line = fgets($handle,1024);
	
	preg_match('/MV: ([^\(^\)]*) (\(\d\d\d\d\))/',$line,$arr);
	
	if(count($arr)>0)
	{
		$foundBudget = false;
		$foundGross =false;
		$gross = -1;
		$budget = -1;
		$date = $arr[2];
		while (substr($line,0,1)!="-")
		{
			$line = fgets($handle,1024);
			if(!$foundBudget)
				preg_match('/BT: ([A-Z]{3}) ([0-9\,]*)/',$line,$money_arr);
			if(!$foundGross)
				preg_match('/GR: (USD) ([0-9\,]*)/',$line,$gross_arr);
			if(count($money_arr)>0)
			{
				//fwrite($handle_write,"BUDGET:".trim($money_arr[1]." ".$money_arr[2])."|");
				$foundBudget = true;
				$budget = trim($money_arr[1]." ".str_replace(",","",$money_arr[2]));
				$money_arr = array();
			}
			if(count($gross_arr)>0)
			{
				//fwrite($handle_write,"GROSS:".trim($gross_arr[1]." ".$gross_arr[2])."|");
				$gross = trim($gross_arr[1]." ".str_replace(",","",$gross_arr[2]));
				$foundGross = true;
				$gross_arr = array();
			}
		}
		if($gross!=-1)
			fwrite($handle_write,trim($arr[1])."|$budget|$gross|$date\n");
	}
	
}
?>