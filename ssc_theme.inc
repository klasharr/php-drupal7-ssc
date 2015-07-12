<?php


function theme_results(array $variables){

  if($variables['error'] == 1){
    return $variables['data'];
  }
  else {
    $output = '<p>';
    foreach($variables['data'] as $result){
      $output .= $result->date . ' ' . l($result->file, $result->link) . '</br>';
    }
    $output .= '</p>';
    return $output;
  }
}