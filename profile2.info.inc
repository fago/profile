<?php
// $Id$

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class Profile2MetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['pid'] += array(
      'validation callback' => 'entity_metadata_validate_integer_positive',
    );

    $properties['uid'] = array(
      'label' => t("User id"),
      'description' => t("The unique ID of the profile owner."),
      'setter callback' => 'entity_metadata_verbatim_set',
      'setter permission' => 'administer profiles',
      'validation callback' => 'entity_metadata_validate_integer_non_negative',
      'clear' => array('user'),
    ) + $properties['uid'];

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the profile."),
      'getter callback' => 'entity_metadata_getter_method',
      'setter callback' => 'entity_metadata_setter_method',
      'setter permission' => 'administer profiles',
      'required' => TRUE,
      'clear' => array('uid'),
    );
    return $info;
  }
}

/**
 * Implements hook_entity_property_info_alter().
 */
function profile2_entity_property_info_alter(&$info) {
  // Add related profiles to the user object.
  $properties = &$info['user']['properties'];
  foreach (profile2_get_types() as $type_name => $type) {
    $properties['profile_' . $type_name] = array(
      'type' => 'profile',
      'label' => t('@type_name profile', array('@type_name' => drupal_ucfirst($type->label))),
      'description' => t("The users's @type_name profile.", array('@type_name' => $type->label)),
      'getter callback' => 'profile2_user_get_properties',
    );
  }
}

