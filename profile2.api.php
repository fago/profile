<?php
// $Id$

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Act on profile being loaded from the database.
 *
 * This hook is invoked during profile loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param $profiles
 *   An array of profiles being loaded, keyed by id.
 */
function hook_profile2_load($profiles) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($profiles)));
  foreach ($result as $record) {
    $profiles[$record->pid]->foo = $record->foo;
  }
}

/**
 * Respond to creation of a new profile.
 *
 * This hook is invoked after the profile is inserted into the database.
 *
 * @param Profile $profile
 *   The profile that is being created.
 */
function hook_profile2_insert($profile) {
  db_insert('mytable')
    ->fields(array(
      'pid' => $profile->pid,
      'extra' => $profile->extra,
    ))
    ->execute();
}

/**
 * Act on a profile being inserted or updated.
 *
 * This hook is invoked before the profile is saved to the database.
 *
 * @param Profile $profile
 *   The profile that is being inserted or updated.
 */
function hook_profile2_presave($profile) {
  $profile->extra = 'foo';
}

/**
 * Respond to updates to a profile.
 *
 * This hook is invoked after the profile has been updated in the database.
 *
 * @param Profile $profile
 *   The profile that is being updated.
 */
function hook_profile2_update($profile) {
  db_update('mytable')
    ->fields(array('extra' => $profile->extra))
    ->condition('pid', $profile->pid)
    ->execute();
}

/**
 * Respond to profile deletion.
 *
 * This hook is invoked after the profile has been removed from the database.
 *
 * @param Profile $profile
 *   The profile that is being deleted.
 */
function hook_profile2_delete($profile) {
  db_delete('mytable')
    ->condition('pid', $profile->pid)
    ->execute();
}

/**
 * Act on profile type being loaded from the database.
 *
 * This hook is invoked during profile type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param $types
 *   An array of profiles being loaded, keyed by profile type names.
 */
function hook_profile2_type_load($types) {
  if (isset($types['main'])) {
    $types['main']->userCategory = FALSE;
    $types['main']->userView = FALSE;
  }
}

/**
 * Respond to creation of a new profile.
 *
 * This hook is invoked after the profile type is inserted into the database.
 *
 * @param Profile $type
 *   The profile type that is being created.
 */
function hook_profile2_type_insert($type) {
  db_insert('mytable')
    ->fields(array(
      'id' => $type->id,
      'extra' => $type->extra,
    ))
    ->execute();
}

/**
 * Act on a profile type being inserted or updated.
 *
 * This hook is invoked before the profile type is saved to the database.
 *
 * @param Profile $type
 *   The profile type that is being inserted or updated.
 */
function hook_profile2_type_presave($type) {
  $type->extra = 'foo';
}

/**
 * Respond to updates to a profile.
 *
 * This hook is invoked after the profile type has been updated in the database.
 *
 * @param Profile $type
 *   The profile type that is being updated.
 */
function hook_profile2_type_update($type) {
  db_update('mytable')
    ->fields(array('extra' => $type->extra))
    ->condition('id', $type->id)
    ->execute();
}

/**
 * Respond to profile type deletion.
 *
 * This hook is invoked after the profile type has been removed from the
 * database.
 *
 * @param Profile $type
 *   The profile type that is being deleted.
 */
function hook_profile2_type_delete($type) {
  db_delete('mytable')
    ->condition('id', $type->id)
    ->execute();
}

/**
 * Define default profile types.
 *
 * @return
 *   An array of default profile types, keyed by profile type names.
 */
function hook_default_profile2_type() {
  $types['main'] = new ProfileType(array(
      'type' => 'main',
      'label' => t('Profile'),
      'weight' => 0,
      'locked' => TRUE,
  ));
  return $types;
}

/**
 * @}
 */
