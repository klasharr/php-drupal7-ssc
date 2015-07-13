<?php

/**
 * @param array $variables
 * @return string
 */
function theme_results_large(array $variables){

  if($variables['error'] == 1){
    return $variables['data'];
  }
  else {

    $header = array('Date', 'Race', 'Series');
    $rows = array();

    foreach($variables['data'] as $result){

      $rows[] = array(
        array('data' => $result->date),
        array('data' => l($result->friendly_file_name, $result->link)),
        array('data' => array_pop($result->path_segments))
        );
    }

    return theme('table', array('header' => $header, 'rows'=> $rows));
  }
}