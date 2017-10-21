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
 * @return array 
 *  user id => user name
 */
function _ssc_get_users_list(){
  $query = db_select('users', 'u');
  $query->condition('u.uid', 0, '<>');
  $query->condition('u.status', 0, '<>');
  $query->fields('u', array('uid', 'name'));
  $result = $query->execute();
  if(!$result){
    return array();
  }

  $options = array();
  foreach ($result as $account) {
    $options[$account->uid] = $account->name;
  }
  return $options;
}
