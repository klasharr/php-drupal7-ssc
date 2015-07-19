<?php

/**
 * Get results listings from webservice.
 *
 * @return array
 */
function _ssc_get_webservice_results_data(){

  $results = &drupal_static(__FUNCTION__);
  if (isset($results)) return $results;

  $webservice_url = variable_get('results_webservice_url');
  if(empty($webservice_url)){
    $results = array(
      'error' => 1,
      'data' => t('The results is URL not configured, see !see',
        array('!see' => l(t('configuration'), 'admin/config/ssc/settings')))
    );
    return $results;
  }

  $result = drupal_http_request(
    variable_get('results_webservice_url')
  );
  if (in_array($result->code, array(200, 304))) {
    $results =  (array) json_decode($result->data);
    return $results;
  }
  else {
    $results = array(
      'error' => 1,
      'data' => t('Results could not be retrieved, HTTP Error code @code. Check the URL in !configuration.',
        array(
          '@code' => $result->code,
          '!configuration' => l(t('configuration'), 'admin/config/ssc/settings'))
      )
    );
    return $results;
  }
}

/**
 * W.I.P.
 */
function _ssc_get_block_slider_images(){

  $nid = variable_get('ssc_slider_nid');
  if(empty($nid)){
    drupal_set_message(t('The image slider block cannot be shown please configure the block and set an image carousel ID'));
  }

  $node =  node_load($nid);

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
    $images[] = image_style_url( variable_get('ssc_slider_image_style','large'), $img['uri']);
  }
  return $images;
}

function _ssc_get_slider_nodes(){
  $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->fields('n', array('type'))
    ->condition('n.type', 'carousel')
    ->execute()
    ->fetchCol();
  if(!$nids){
    return array();
  }

  $nodes = node_load_multiple($nids);
  $out = array();
  foreach($nodes as $id => $node){
    $out[$id] = sprintf("%s [%d]", $node->title,$id);
  }
  return $out;
}