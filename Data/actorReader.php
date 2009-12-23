<?php
echo "hello";

//$PATH = "C:\Users\Adam\Documents";
$PATH = "C:\Documents and Settings\ashepard\My Documents";

$businesshandle;
if(file_exists("$PATH\Visual Studio 2008\Projects\MovieVis\Data\movie_gross2.dat"))
	$businesshandle = fopen("$PATH\Visual Studio 2008\Projects\MovieVis\Data\movie_gross2.dat","r");
else
	echo "no file!";

$movieNameArray=array();
$count=0;
$line = fgets($businesshandle,1024);
while(!feof($businesshandle))
{
	
	$line = fgets($businesshandle,1024);
	//echo $line;
	preg_match_all('/([^|]*)|[^|]*|[^|]*|([0-9]*)/',$line,$arr);
	//print_r($arr);
	if(count($arr)>0)
	{
		//echo "in!";
		$name = $arr[1][0];
		$date = trim($arr[1][6]);
		//print_r($arr);
		//echo $name."-$date\n";
		if($name!="" && $name!=" ")
			$movieNameArray[$name][$date]=array("value"=>1,"director"=>array());
	}
	$count++;
}


echo "loaded ".count($movieNameArray)." movies that I know about.\n";
//var_dump($movieNameArray);

if(file_exists("$PATH\actors.list\actors.list"))
	$handle = fopen("$PATH\actors.list\actors.list","r");
else
	echo "no directors file!";
$handle_write =fopen("$PATH\Visual Studio 2008\Projects\MovieVis\Data\actors.massaged","w");
//echo $handle;
//var_dump($handle);

$directorsArray=array();

$count =0;

echo "where am I";
while (!feof($handle))
{
	if($count%10000 == 0)
	{
		
		echo "coutn: $count\n";
	}
	$movie = "";
	$director = "";
	$line = fgets($handle,1024);
	//echo $line;	
	preg_match('/([a-zA-Z\.0-9\-\']*, [a-zA-Z\.0-9\-\' \(\)]*)\s*([a-zA-z\-\& \'\:]*) \((\d\d\d\d)\).*/',$line,$arr);
	if(count($arr)>0)
	{
		//print_r($arr);
		$director = $arr[1];
		$dirarr=explode(",",$director);
		$director = $dirarr[0];
		$da=explode("(",$dirarr[1]);
		$director=trim($da[0])." ".trim($director);
		$movie = trim($arr[2]);
		$date = trim($arr[3]);
		if($director=="Jackson, Peter" || substr($line,0,7)=="Jackson")
			echo "found peter jackson!-$director\n";
		//echo "$movie-$director-$date-$count\n";
		if(array_key_exists($movie,$movieNameArray))
		{
			
			//echo "Found movie I know about - ".$movie;
			
			if(array_key_exists($date,$movieNameArray[$movie]))
			{
				//echo ", Found right year: ".$date.", adding director:$director";
				$movieNameArray[$movie][$date]["director"][]=$director;
				
			}
			//else
			//	echo ", wrong year";
			//echo "\n";
		}
		
		$test2 = "\t";
		$line2 = "";
		$continue=true;
		while(!feof($handle) && $continue)
		{
			$line2 = fgets($handle,1024);
			$test2 = substr($line2,0,1);
			
			preg_match('/(\s{1,})([a-zA-z\-\& \'\:]*) \((\d\d\d\d)\).*/',$line2,$arr);
			if(count($arr)>0)
			{
				//print_r($arr);
				
				$movie = trim($arr[2]);
				$date = trim($arr[3]);
				
				//echo "INNER:$movie-$director-$date-\"$test2\"\n";
				if(array_key_exists($movie,$movieNameArray))
				{
					
					//echo "Found movie I know about - ".$movie;
					
					if(array_key_exists($date,$movieNameArray[$movie]))
					{
						//echo ", Found right year: ".$date.", adding director:$director";
						$movieNameArray[$movie][$date]["director"][]=$director;
						
					}
					//else
					//	echo ", wrong year";
					//echo "\n";
				}
			}
			else
				$continue=false;
		}
		preg_match('/([a-zA-Z\.0-9\-\']*, [a-zA-Z\.0-9\-\' \(\)]*)\s*([a-zA-z\-\& \'\:]*) \((\d\d\d\d)\).*/',$line2,$arr);
		if(count($arr)>0)
		{
			//print_r($arr);
			//$director=trim($arr[1]);
			$movie = trim($arr[1]);
			$date = trim($arr[3]);
			
			//echo "POST:$movie-$director-$date-\"$test2\"\n";
			if(array_key_exists($movie,$movieNameArray))
			{
				
				//echo "Found movie I know about - ".$movie;
				
				if(array_key_exists($date,$movieNameArray[$movie]))
				{
					//echo ", Found right year: ".$date.", adding director:$director";
					$movieNameArray[$movie][$date]["director"][]=$director;
					
				}
				//else
				//	echo ", wrong year";
				//echo "\n";
			}
		}
		
		$count++;		
	}
}

foreach($movieNameArray as $key=>$value)
{
	//fwrite($handle_write,"$key|");
	foreach($value as $year=>$values)
	{
		//fwrite($handle_write,$key."|".$year."|");
		foreach($values["director"] as $dKey=>$director)
		{
			fwrite($handle_write,$key."|".$year."|".$director."\n");
		}
		//print_r($values);
	}
	//fwrite($handle_write,"\n");	
}
//print_r($movieNameArray["Dirge to the Sea"]);
?>