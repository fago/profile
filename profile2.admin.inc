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
  
  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows = array();
  
  foreach($types as $id => $type) {
    $type_url_str = str_replace('_', '-', $id);

    $row[] = array('data' => theme('profile2_admin_type', array('label' => $type->label, 'type' => $type)));
    $row[] = array('data' => l(t('edit'), 'admin/structure/profiles/manage/' . $type_url_str));
    // Set the delete column.
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
  if (!isset($type->name)) {
    // @todo!
    // This is a new type. Node module managed types are custom and unlocked.
    //$type = node_type_set_defaults(array('custom' => 1, 'locked' => 0));
  }
  
  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Identification'),
  );
  
  return $form;
}
