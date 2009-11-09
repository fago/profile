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

    $row = array();
    $row[] = array('data' => $type['label']);
    $row[] = array('data' => l(t('edit'), 'admin/structure/profiles/manage/' . $type_url_str));
    // Set the delete column.
    if (!$type['locked']) {
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

  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows
  );
  
  return $build;

}
