<?php

function form_ssc_main_settings($form, &$form_state) {

  if (variable_get('cache', 0)) {

    $form['cache'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cache clear'),
      '#weight' => 0,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['cache']['cache_clear_description'] = array(
      '#title' => t('Clear path cache'),
      '#type' => 'item',
      '#description' => t(
        'Use this button to force a cache clear. It may take a few seconds'
      )
    );

    $form['cache']['clear'] = array(
      '#type' => 'submit',
      '#description' => 'foo',
      '#value' => t('Clear page cache'),
      '#submit' => array('_ssc_page_clear_cache_submit'),
    );
  }

  $form['generic_member'] = array(
    '#type' => 'fieldset',
    '#title' => t('Generic member'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['generic_member']['ssc_members_only_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Member only page paths'),
    '#description' => t(
        'Enter paths or path wildcards which should be members only pages, meaning access is granted only when authenticated. e.g. members/* . Have one entry per line.'
      ) .
      t(
        'If logged out users try to get to one of these pages then they will see the \'members only warning message\' defined below.'
      ),
    '#rows' => 3,
    '#default_value' => variable_get('ssc_members_only_paths'),
  );

  $form['generic_member']['ssc_default_minutes_page'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#maxlength' => 40,
    '#title' => t('Member only page paths'),
    '#description' => t(
      'Enter the default minutes page. e.g. members/minutes/2014. '
    ),
    '#default_value' => variable_get('ssc_default_minutes_page'),
  );

  $query = db_select('users', 'u');
  $query->condition('u.uid', 0, '<>');
  $query->condition('u.status', 0, '<>');
  $query->fields('u', array('uid', 'name'));
  $result = $query->execute();

  $options = array();
  foreach ($result as $account) {
    $options[$account->uid] = $account->name;
  }

  $form['generic_member']['ssc_generic_user_id'] = array(
    '#title' => t('Generic member'),
    '#type' => 'select',
    '#options' => $options,
    '#description' => t(
      'Choose a user to be the generic user. The generic user is a \'not editable for non admins account\'. This user cannot trigger password resets.'
    ),
    '#default_value' => variable_get('ssc_generic_user_id'),
    '#element_validate' => array('form_ssc_main_setting_generic_user_id_validate'),
  );

  $form['ssc_members_only_message'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('Members only warning page message'),
    '#description' => t(
      'Enter a message which will be shown to users if they try to view a members only page.'
    ),
    '#default_value' => variable_get('ssc_members_only_message'),
  );

  $form['meta'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta descriptions and page titles'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['meta']['ssc_front_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Home page title tag'),
    '#description' => t('Enter the title tag text for the home page which is Drupal <front>'),
    '#size' => 100,
    '#default_value' => variable_get('ssc_front_title'),
  );

  $form['meta']['ssc_front_meta_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Front page meta description'),
    '#description' => t('Enter a meta description for the home page which is Drupal <front>'),
    '#rows' => 2,
    '#default_value' => variable_get('ssc_front_meta_desc'),
  );

  $form['results'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for the race results block panels'),
    '#weight' => 20,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['results']['results_webservice_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Webservice results URL'),
    '#description' => t('Enter a URL, this should return compliant results listings data'),
    '#size' => 100,
    '#default_value' => variable_get('results_webservice_url'),
  );

  return system_settings_form($form);
}

function form_ssc_main_setting_generic_user_id_validate($element, &$form_state) {
  if (isset($element['#value'])) {
    if ((int) $element['#value'] == 0) {
      form_error($element, t('Please enter an integer value greater than 0.'));
    }
    if (!(bool) db_select('users')->fields('users', array('uid'))->condition(
      'uid',
      (int) $element['#value']
    )->execute()->fetchField()
    ) {
      form_error($element, t('You must enter the ID of a valid user.'));
    }
  }

  return $element;
}

/**
 * @see ssc_menu
 * @param $type
 */
function _ssc_page_forward($type) {
  if ($type == 'login') {
    drupal_goto('user/login');
  }
  else {
    drupal_goto('user/logout');
  }
}

/**
 * @see ssc_menu_alter()
 * @param $account
 * @return bool
 */
function _ssc_user_edit_access_callback($account) {

  if (user_access('administer users')) {
    return TRUE;
  }

  if (($GLOBALS['user']->uid == $account->uid) &&
    ($account->uid != variable_get('ssc_generic_user_id', 999999))
  ) {
    return TRUE;
  }

  return FALSE;
}

function _ssc_form_clear_path_cache() {

  $form['description'] = array(
    '#markup' => sprintf(
      '<p>%s</p><p>%s</p>',
      t(
        'Pages on this site are fully cached for logged out users. Therefore, users might not see recent
            changes if pages are in cache.'
      ),
      t('Use this button to force a cache clear. It may take a few seconds')
    )
  );
  $form['clear_cache']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear page cache'),
    '#submit' => array('_ssc_page_clear_cache_submit'),
  );

  return $form;
}

function _ssc_page_clear_cache_submit($form, &$form_state) {
  cache_clear_all('*', 'cache_page', TRUE);
  drupal_set_message(t('Cached pages cleared.'));
}

function _ssc_page_clear_access_callback() {
  if (!variable_get('cache', 0)) {
    return FALSE;
  }

  return user_access('ssc clear page cache');
}

function _ssc_page_secure_redir() {

  $url = isset($_GET['d']) ? $_GET['d'] : NULL;
  if (empty($url) || $url == 'jump') {
    drupal_goto();
  }

  if (!drupal_match_path($url, variable_get('ssc_members_only_paths', NULL))) {
    drupal_not_found();
    exit();
  }

  if (user_is_logged_in()) {
    drupal_goto($url);
  }

  drupal_set_message(
    t(
      'You a requesting a members only page, please login and you will be redirected to it.'
    )
  );
  drupal_goto('user/login', array('query' => array('destination' => $url)));
}

/**
 * @todo reconcile with function below to have just one validator for either large or small panel
 * @param $element
 * @param $form_state
 */
function _ssc_validate_block_settings_latest_results_small_panel_count($element, &$form_state) {

  if ((int) $element['#value'] == 0) {
    form_error($element, t('Please enter an integer value between 1 and 10.'));
  }
}

function _ssc_validate_block_settings_latest_results_large_panel_count($element, &$form_state) {

  if ((int) $element['#value'] == 0) {
    form_error($element, t('Please enter an integer value between 1 and 30.'));
  }
}

/**
 * @param $variables
 */
function _ssc_log_results_panel_error($variables) {

  watchdog(
    'ssc',
    'Results webservice: ' . variable_get('results_webservice_url') .
    ' Error: ' . $variables['data']['data']
  );
}
