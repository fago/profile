<?php


interface EntityCRUDControllerInterface extends DrupalEntityControllerInterface {
  
  /**
   * Delete permanently saved entities.
   *
   * @param $ids
   *   An array of entity IDs.
   */
  public function delete($ids);
}


class EntityCRUDController extends DrupalDefaultEntityController implements EntityCRUDControllerInterface {

  public function load($ids = array(), $conditions = array()) {
    return parent::load($ids, $conditions);
    //TODO: unserialize if necessary
  }
  
  public function delete($ids) {
    $entities = $this->load($ids);
  
    db_delete($this->entityInfo['base table'])
      ->condition($this->idKey, $ids, 'IN')
      ->execute();

    foreach ($entities as $entity) {
      $entity->invoke('delete');
    }
    $this->resetCache();
  }
}

/**
 * Delete permanently saved entities.
 *
 * @param $entity_type
 *   The entity type to delete, e.g. node or user.
 * @param $ids
 *   An array of entity IDs.
 */
function entity_delete($entity_type, array $ids) {
  entity_get_controller($entity_type)->delete($ids);
}


/**
 * Provides a unique interface for CRUD operations on entites.
 */
interface EntityCRUDInterface {

  /**
   * Permanently save this entity.
   *
   * @return
   *   Failure to write a record will return FALSE. Otherwise SAVED_NEW or
   *   SAVED_UPDATED is returned depending on the operation performed.
   */
  public function save();

  /**
   * Permanently delete this entity.
   */
  public function delete();

  /**
   * Returns the identifier of this instance.
   *
   * @return
   *   The identifier or NULL, if this entity hasn't been saved yet.
   */
  public function identifier();

  /**
   * Returns the entity type of this instance.
   *
   * @return
   *   The name of the entity type of this instance.
   */
  public function entityType();

  /**
   * Returns the entity info for this instance as returned by entity_get_info().
   *
   * @return
   *   An array of entity info for this entity type.
   *
   * @see
   *   entity_get_info()
   */
  public function entityInfo();

}


class EntityDB extends FacesExtendable implements EntityCRUDInterface {

  protected $entityType;
  protected $entityInfo;
 
  public function __construct($entityType) {
    $this->entityType = $entityType;
    $this->entityInfo = entity_get_info($entityType);
  }
  
  public function identifier() {
    list($id, , ,) = entity_extract_ids($this->entityType, $this);
    return $id;
  }

  public function entityInfo() {
    return $this->entityInfo;
  }

  public function entityType() {
    return $this->entityType;
  }

  public function save() {
    $this->invoke('presave');
    $info = $this->entityInfo();

    if ($this->identifier() !== NULL) {
      $return = drupal_write_record($info['base table'], $this, $info['object keys']['id']);
      $this->invoke('update');
    }
    else {
      $return = drupal_write_record($info['base table'], $this);
      $this->invoke('insert');
    }
    return $return;
  }

  public function delete() {
    db_delete($this->entityInfo['base table'])
      ->condition($this->idKey, $id)
      ->execute();
    $this->invoke('delete');
    entity_get_controller($this->entityType)->resetCache();
  }

  /**
   * Invokes a hook and calls any field API attachers.
   */
  protected function invoke($hook) {
    $info = $this->entityInfo();
    if (!empty($info['fieldable']) && function_exists($function = 'field_attach_' . $hook)) {
      $function($this);
    }
    module_invoke_all($this->entityType . '_' . $hook, $this);
  }

}


class Profile extends EntityDB {
  
  
  
}

