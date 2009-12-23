<?php
echo "hello";

//$PATH = "C:\Users\Adam\Documents";
$PATH = "C:\Documents and Settings\ashepard\My Documents";

$businesshandle;
if(file_exists("$PATH\Visual Studio 2008\Projects\MovieVis\Data\movie_gross_500.dat"))
	$businesshandle = fopen("$PATH\Visual Studio 2008\Projects\MovieVis\Data\movie_gross_500.dat","r");
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
			$movieNameArray[$name][$date]=array("value"=>1,"director"=>array(),"releaseDate"=>-1);
	}
	$count++;
}


echo "loaded ".count($movieNameArray)." movies that I know about.\n";
//var_dump($movieNameArray);

if(file_exists("$PATH".'\release-dates.list\release-dates.list'))
	$handle = fopen("$PATH".'\release-dates.list\release-dates.list',"r");
else
{
	echo "no directors file!".'$PATH\release-dates.list\release-dates.list';
	exit(0);
}
echo "I will write to: $PATH\Visual Studio 2008\Projects\MovieVis\Data\releaseDates.massaged";
$handle_write =fopen("$PATH".'\Visual Studio 2008\Projects\MovieVis\Data\releaseDates.massaged',"w");
//echo $handle;
//var_dump($handle);

$directorsArray=array();

$count =0;

echo "where am I";
while (!feof($handle))
{

	$movie = "";
	$director = "";
	$line = fgets($handle,1024);
	//echo $line;	
	preg_match('/(.*) \((\d\d\d\d)\)\s*USA:([^\(]*)\s*/',$line,$arr);
	if(count($arr)>0)
	{
		if($count%10000==0)
			print_r($arr);
		$movie = trim($arr[1]);
		$year = trim($arr[2]);
		$releaseDate = trim($arr[3]);
		
	
		//echo "$movie-$director-$date-$count\n";
		if(array_key_exists($movie,$movieNameArray))
		{
			
			echo "Found movie I know about - ".$movie;
			
			if(array_key_exists($year,$movieNameArray[$movie]))
			{
				echo ", Found right year: ".$year.", adding releaseDate:$releaseDate";
				$movieNameArray[$movie][$year]["releaseDate"]=$releaseDate;
				
			}
			else
				echo ", wrong year, $year vs. ".implode(",",array_keys($movieNameArray[$movie]));
			echo "\n";
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
		//foreach($values["director"] as $dKey=>$director)
		//{
			fwrite($handle_write,$key."|".$year."|".$values["releaseDate"]."\n");
		//}
		//print_r($values);
	}
	//fwrite($handle_write,"\n");	
}
//print_r($movieNameArray["Dirge to the Sea"]);
?>