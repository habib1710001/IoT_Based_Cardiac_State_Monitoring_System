<?php
session_start();

if (!$_SESSION["useruid"]) {
    header("location: http://kusumtara.com/login.php");
    exit();
}

$servername = "localhost";

// REPLACE with your Database name
$dbname = "u422366820_ece_iot";
// REPLACE with Database user
$username = "u422366820_kusum_tara";
// REPLACE with Database user password
$password = "kusumTara2022";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, value1, reading_time FROM SensorData order by id desc limit 1500";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'value2');
$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

/*echo $value1;
echo $value2;
echo $value3;
echo $reading_time;*/

$result->free();
$conn->close();
?>

<!DOCTYPE html>
<html>
   <head>
      
      <script type = "text/JavaScript">
         <!--
            function AutoRefresh( t ) {
               setTimeout("location.reload(true);", t);
            }
         //-->
      </script>
      
   </head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <style>
    body {
      min-width: 310px;
      max-width: 1280px;
      height: 500px;
      margin: 0 auto;
    }
  </style>


<body onload = "JavaScript:AutoRefresh(5000);">
<h2 style="text-align:center; margin-left: 10%; border-radius: 0px; margin:0px;  background-color:Red; border: 2px red;border-radius: 5px; margin-left:20%; margin-right:20%; color:White;">ECG Monitor</h2>

<a href="home.php" style=" text-decoration: none ;font-size: 1.2rem; margin:20px;  background-color:Red; color:white; border: 5px red; border-radius: 2px;">Back</a>

<div id="chart-ecg" class="container"></div>
    
    
<script type = "text/JavaScript">

var value1 = <?php echo $value1; ?>;
var reading_time = <?php echo $reading_time; ?>;

var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-ecg' },
  title: { text: 'Lead(I)' },
  series: [{
    showInLegend: false,
    data: value1
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: false }
    },
    series: { color: '#FF0000' }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Voltage' }
  },
  credits: { enabled: false }
});
</script>
</body>
</html>