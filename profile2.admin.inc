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

    // Start building the row and add a name label and edit operation link
    $row = array();
    $row[] = array('data' => theme('profile2_admin_type', array('label' => $type->label, 'type' => $type)));
    $row[] = array('data' => l(t('edit'), 'admin/structure/profiles/manage/' . $type_url_str));

    // If not locked, add a delete column
    if (!$type->locked) {
      $row[] = array('data' => l(t('delete'), 'admin/structure/profiles/manage/' . $type_url_str . '/delete'));
    }
    else {
      $row[] = array('data' => '');
    }

    // Add this row onto the table
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No profiles available. <a href="@link">Add a profile</a>.', array('@link' => url('admin/structure/profiles/add'))), 'colspan' => '5', 'class' => array('message')));
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
  $profile_type  = $variables['type'];

  $output = check_plain($label);
  $output .= ' <small> (Machine name: ' . check_plain($profile_type->type) . ')</small>';
  // @todo if we add a description column to profile types.
  //$output .= '<div class="description">' . filter_xss_admin($type['description']) . '</div>';
  return $output;
}

/**
 * Generates the profile type editing form.
 */
function profile2_type_form($form, &$form_state, $profile_type = NULL) {
  if (!isset($profile_type)) {
    // This is a new type.
    $profile_type = new ProfileType();
  }

  // Make the type object available to implementations of hook_form_alter.
  $form_state += array('profile_type' => $profile_type);

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $profile_type->label,
    '#description' => t('The human-readable name of this profile type. This text will be displayed as part of the list on the <em>TO DO</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 30,
    '#field_suffix' => ' <small id="edit-name-suffix">' . ($profile_type->locked ? t('Machine name: @name', array('@name' => $profile_type->type)) : '&nbsp;') . '</small>',
  );

  // Machine-readable type
  $form['type'] = array(
    '#title' => t('Type'),
    '#type' => 'textfield',
    '#description' => t('Computer-readable type. Use only letters and numbers. This must also be unique.'),
    '#required' => TRUE,
    '#size' => 20,
    '#default_value' => $profile_type->type,
  );

  // If the profile is locked, we don't want people seeing or editing the
  // machine-readable type: no point
  if ($profile_type->locked) {
    $form['type']['#type'] = 'value';
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#weight' => 40,
  );

  if (!$profile_type->locked) {
    if (!empty($profile_type->type)) {
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete profile type'),
        '#weight' => 45,
      );
    }
  }

  return $form;
}

/**
 * Form API submit callback for profile2_type_form
 */
function profile2_type_form_submit(&$form, &$form_state) {
  // Grab profile_type back ouut of the $form array
  $profile_type = $form_state['profile_type'];

  // Set the name and, if it's not set (and the type isn't locked)
  // also set the machine-readable profile type
  $profile_type->label = $form_state['values']['name'];
  if (!$profile_type->locked && empty($profile_type->type)) {
    $profile_type->type = $form_state['values']['type'];
  }

  // Save and go back
  $profile_type->save();
  drupal_goto('admin/structure/profiles');
}
