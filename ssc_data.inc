<?php

/**
 * Get results listings from webservice.
 *
 * @param int $count
 * @return array|mixed
 */
function _ssc_get_webservice_results_data($count = 20){

  $webservice_url = variable_get('results_webservice_url');
  if(empty($webservice_url)){
    return array(
      'error' => 1,
      'data' => t('The results is URL not configured, see !see',
        array('!see' => l(t('configuration'), 'admin/config/ssc/settings')))
    );
  }

  $result = drupal_http_request(
    variable_get('results_webservice_url')
  );
  if (in_array($result->code, array(200, 304))) {
    return (array) json_decode($result->data);
  }
  else {
    return array(
      'error' => 1,
      'data' => t('Results could not be retrieved, HTTP Error code @code. Check the URL in !configuration.',
        array(
          '@code' => $result->code,
          '!configuration' => l(t('configuration'), 'admin/config/ssc/settings'))
      )
    );
  }
}

/**
 * W.I.P.
 */
function _ssc_get_block_slider_images(){

  $node =  node_load(297);

  if(!$node || $node->type != 'carousel'){
    watchdog('ssc','Slider block loading invalid node');
    return;
  }

  if(empty($node->field_image['und'])){
    watchdog('ssc','Slider block node has no images');
    return;
  }
  $images = array();

  foreach($node->field_image['und'] as $img){
    $images[] = image_style_url('large', $img['uri']);
  }
  return $images;
}