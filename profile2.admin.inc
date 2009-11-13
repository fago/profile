<?php
// $Id$

/**
 * @file
 * Profile type editing UI.
 */

/**
 * Displays the profile type admin overview page.
 */
function profile2_overview_types() {
  $types = profile2_get_types();
  $field_ui = module_exists('field_ui');  

  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2'));
  $rows = array();
  
  foreach($types as $id => $type) {
    $type_url_str = str_replace('_', '-', $id);

    // Label and machine name.
    $row[] = array('data' => theme('profile2_admin_type', array('label' => $type->label, 'type' => $type)));
    // Edit type.
    $row[] = array('data' => l(t('edit'), 'admin/structure/profiles/manage/' . $type_url_str));
    if ($field_ui) {
      // Manage fields.
      $row[] = array('data' => l(t('manage fields'), 'admin/structure/profiles/manage/' . $type_url_str . '/fields'));

      // Display fields.
      $row[] = array('data' => l(t('manage display'), 'admin/structure/profiles/manage/' . $type_url_str . '/display'));
    }
    // Delete type.
    if (!$type->locked) {
      $row[] = array('data' => l(t('delete'), 'admin/structure/profiles/manage/' . $type_url_str . '/delete'));
    }
    else {
      $row[] = array('data' => '');
    }
    
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No profile types available. <a href="@link">Add profile type</a>.', array('@link' => url('admin/structure/profiles/add'))), 'colspan' => '5', 'class' => array('message')));
  }

  $build['profile_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows
  );
  return $build;
}

/**
 * Theme the label and machine name for the profile type admin list.
 */
function theme_profile2_admin_type($variables) {
  $label = $variables['label'];
  $type  = $variables['type'];

  $output = check_plain($label);
  $output .= ' <small> (Machine name: ' . check_plain($type->name) . ')</small>';
  // @todo if we add a description column to profile types.
  //$output .= '<div class="description">' . filter_xss_admin($type['description']) . '</div>';
  return $output;
}

/**
 * Generates the profile type editing form.
 */
function profile2_type_form($form, &$form_state, $type = NULL) {
  // @todo: set defaults for new type.
  if (!isset($type->name)) {
    // This is a new type. Node module managed types are custom and unlocked.
    //$type = node_type_set_defaults(array('custom' => 1, 'locked' => 0));
  }
  
  // @todo: create form
  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Identification'),
  );
  
  $form['identity']['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $type->label,
    '#description' => t('The human-readable name of this profile type. This text will be displayed as the tab title on the profile page, and when users add information about themselves. It is recommended that this name begin with a capital letter and contain only letters, numbers, and <strong>spaces</strong>. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 30,
    '#field_suffix' => ' <small id="edit-name-suffix">' . ($type->locked ? t('Machine name: @name', array('@name' => $type->name)) : '&nbsp') . '</small>',
  );

  if (!$type->locked) {
    $js_settings = array(
      'type' => 'setting',
      'data' => array(
        'machineReadableValue' => array(
          'name' => array(
            'text' => t('Machine name'),
            'target' => 'name',
            'searchPattern' => '[^a-z0-9]+',
            'replaceToken' => '_',
          ),
        ),
      ),
    );
    $form['identity']['name'] = array(
      '#title' => t('Machine name'),
      '#type' => 'textfield',
      '#default_value' => $type->name,
      '#maxlength' => 32,
      '#required' => TRUE,
      '#description' => t('The machine-readable name of this content type. This text will be used for constructing the URL of the <em>add new content</em> page for this content type. This name must contain only lowercase letters, numbers, and underscores. Underscores will be converted into hyphens when constructing the URL of the <em>add new content</em> page. This name must be unique.'),
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'system') . '/system.js', $js_settings),
      ),
    );
  }
  else {
    $form['identity']['type'] = array(
      '#type' => 'value',
      '#value' => $type->name,
    );
  }
  
  
  return $form;
}
