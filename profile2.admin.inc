<?php
// $Id$

/**
 * @file
 * Profile type editing UI.
 */

/**
 * Form builder to display the profile type admin overview page and set weights.
 *
 * @ingroup forms
 */
function profile2_overview_types($form) {
  $types = profile2_get_types();
  $field_ui = module_exists('field_ui');  

  $form['#tree'] = TRUE;  // ???
  foreach($types as $id => $type) {
    $type_url_str = str_replace('_', '-', $id);    
    $form[$id]['#profile_type'] = $type;
    $form[$id]['identification'] = array('#theme' => 'profile2_admin_type');
    $form[$id]['identification']['label'] = array('#markup' => check_plain($type->label));
    $form[$id]['identification']['name'] = array('#markup' => check_plain($id));
    $form[$id]['weight'] = array('#type' => 'weight', '#delta' => 10, '#default_value' => $type->weight);
    $form[$id]['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => 'admin/structure/profiles/manage/' . $type_url_str);
    if ($field_ui) {
      // Manage fields.
      $form[$id]['manage_fields'] = array('#type' => 'link', '#title' => t('manage fields'), '#href' => "admin/structure/profiles/manage/$type_url_str/fields");
      // Display fields.
      $form[$id]['manage_display'] = array('#type' => 'link', '#title' => t('display fields'), '#href' => "admin/structure/profiles/manage/$type_url_str/display");
    }
    // Delete type.
    if (!$type->locked) {
      $form[$id]['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => "admin/structure/profiles/manage/$type_url_str/delete");
    }
    
  }

  // Only make this form include a submit button and weight if more than one
  // profile type exists.
  if (count($types) > 1) {
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  }
  elseif (isset($type)) {
    unset($form[$id]['weight']);
  }
  
  return $form;
}

/**
 * Theme the profile type overview as a sortable list of types.
 *
 * @ingroup themeable
 * @see profile2_overview_types()
 */
function theme_profile2_overview_types($variables) {
  $form = $variables['form'];
  $field_ui = module_exists('field_ui');  

  $rows = array();
  
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#profile_type'])) {
      // Note $type is a piece of form here; 
      // find the actual object at $type['#profile_type']
      $type = &$form[$key];
      
      $row = array();
      $row[] = drupal_render($type['identification']);
            
      if (isset($type['weight'])) {
        $type['weight']['#attributes']['class'] = array('type-weight');
        $row[] = drupal_render($type['weight']);
      }
      $row[] = drupal_render($type['edit']);
      
      if ($field_ui) {
        $row[] = drupal_render($type['manage_fields']);
        $row[] = drupal_render($type['manage_display']);
      }
      if (!$type['#profile_type']->locked) {
        $row[] = drupal_render($type['delete']);
      }
      else {
        $row[] = array('data' => '');
      }
      
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }
  
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No profile types available. <a href="@link">Add profile type</a>.', array('@link' => url('admin/structure/profiles/add'))), 'colspan' => '5', 'class' => array('message')));
  }
  
  $header = array(t('Name'));
  if (isset($form['submit'])) {
    $header[] = t('Weight');
    drupal_add_tabledrag('profile_types', 'order', 'sibling', 'type-weight');
  }
  $header[] = array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2');
  
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'profile_types'))) . drupal_render_children($form);
}

/**
 * Theme the label and machine name for the profile type admin list.
 *
 * @param $variables
 *  Theme variables, containing in single key 'form' the name and label form
 *  elements to render.
 */
function theme_profile2_admin_type($variables) {
  $label = $variables['form']['label'];
  $name  = $variables['form']['name'];

  $output = drupal_render($label);
  $output .= ' <small> (Machine name: ' . drupal_render($name) . ')</small>';
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
