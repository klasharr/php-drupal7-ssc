<?php

/**
 * @param array $variables
 * @return string
 */
function theme_results_large(array $variables){

  if($variables['data']['error'] == 1){
    _ssc_log_results_panel_error($variables);
    return t('Error retrieving results');
  }
  else {

    if(empty($variables['data']['data'])) return t('No results data');

    $header = array('Date', 'Race', 'Series');
    $rows = array();

    $tmp = array_slice($variables['data']['data'], 0, $variables['display_count']);

    foreach($tmp as $result){

      $rows[] = array(
        array('data' => $result->date),
        array('data' => l($result->friendly_file_name, $result->link)),
        array('data' => array_pop($result->path_segments))
        );
    }

    return theme('table', array('header' => $header, 'rows'=> $rows));
  }
}

/**
 * @param array $variables
 * @return string
 */
function theme_results_small(array $variables){

  if($variables['data']['error'] == 1){
    _ssc_log_results_panel_error($variables);
    return t('Error retrieving results');
  }
  else {

    if(empty($variables['data']['data'])) return t('No results data');

    $tmp = array_slice($variables['data']['data'], 0, $variables['display_count']);

    $items = array();
    $output = '';

    foreach($tmp as $result){
      $items[] = array(
        'data' => $result->date.'<br/>'.l(array_pop($result->path_segments). ' > '. $result->friendly_file_name, $result->link),
      );
    }
    $output .= '<div class="item-list">';
    $output .= theme_item_list(
      array(
        'title' => null,
        'items' => $items,
        'type' => 'ul',
        'attributes' => array('id' => 'latest_race_results')
      )
    );
    $output .= '</div>';
    $output .= sprintf('<div class="more-link">%s</div>',l(t('more'),variable_get('results_more_url')));

    return $output;
  }
}

/**
 * W.I.P
 *
 * @param $data
 * @return string
 */
function theme_ssc_carousel($data){

  $output = '<div class="wrapper">
    <div class="jcarousel-wrapper">
       <div class="jcarousel">
            <ul>';
  foreach($data['images'] as $url){
    $output .= sprintf( '<li><img width="500" src="%s"></li>', $url);
  }
  $output .= '</ul>
       </div>
    <a href="#" class="jcarousel-control-prev">&lsaquo;</a>
    <a href="#" class="jcarousel-control-next">&rsaquo;</a>
    <p class="jcarousel-pagination"></p>
    </div>
</div>';

  return $output;
}