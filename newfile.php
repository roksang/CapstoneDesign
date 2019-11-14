<?php
$db_host ="localhost";
$db_user ="root";
$db_passwd="123456";
$db_name="test";
$Buffer=NULL;
$conn=mysqli_connect($db_host,$db_user,$db_passwd,$db_name);

date_default_timezone_set('Asia/Seoul');
$now = new DateTime();

if(mysqli_connect_errno()){
 echo "MySQL 연결 오류: " .mysqli_connect_error();
}

$a=21;
$Buffer = $_GET["Buffer"];

echo "Waterlevel test<br>";
echo "Buffer=";
echo $a-$Buffer;
echo "<br>";

$dates = $now->format("Y-m-d");
echo "date=";
echo $dates;
echo "<br>";

$times = $now->format("H:i:s");
echo "time=";
echo $times;
echo "<br>";

$sql = "insert into waterlevel(Buffer, days, times) values($a-$Buffer, '$dates', '$times')";
mysqli_query($conn, $sql);
mysqli_close($conn);
sleep(1800);
?>