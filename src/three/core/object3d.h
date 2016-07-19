#ifndef THREE_OBJECT3D_H
#define THREE_OBJECT3D_H

#include "../math/math_forword_declar.h"
#include "../math/math.hpp"

#include "../math/vector3.h"
#include "../math/euler.h"
#include "../math/quaternion.h"
#include "../math/matrix3.h"
#include "../math/matrix4.h"
#include "layers.h"

namespace three {

class Vector3;
class Euler;
class Quaternion;
class Matrix4;
class Matrix3;
class Layers;

class Object3D
{
    // Q_GADGET
public:
    Object3D()
    {
        this->id = Object3D::Object3DIdCount++;
        this->uuid = three::Math::generateUUID();
        this->name = '';
        this->type = 'Object3D';

        this->parent = nullptr;
        // this->children = [];
        this->children.clear();

        this->up = Object3D::DefaultUp.clone();

        //        var position = new THREE.Vector3();
        //        var rotation = new THREE.Euler();
        //        var quaternion = new THREE.Quaternion();
        //        var scale = new THREE.Vector3( 1, 1, 1 );

        //        function onRotationChange() {

        //            quaternion.setFromEuler( rotation, false );

        //        }

        //        function onQuaternionChange() {

        //            rotation.setFromQuaternion( quaternion, undefined, false );

        //        }

        //        rotation.onChange( onRotationChange );
        //        quaternion.onChange( onQuaternionChange );

        // TODO
        //        Object.defineProperties( this, {
        //                position: {
        //                    enumerable: true,
        //                    value: position
        //                }
        //                rotation: {
        //                    enumerable: true,
        //                    value: rotation
        //                }
        //                quaternion: {
        //                    enumerable: true,
        //                    value: quaternion
        //                }
        //                scale: {
        //                    enumerable: true,
        //                    value: scale
        //                }
        //                modelViewMatrix: {
        //                    value: new THREE.Matrix4()
        //                }
        //                normalMatrix: {
        //                    value: new THREE.Matrix3()
        //                }
        //            } );

        this->rotationAutoUpdate = true;

        //        this->matrix = new THREE.Matrix4();
        //        this->matrixWorld = new THREE.Matrix4();

        this->matrixAutoUpdate = Object3D::DefaultMatrixAutoUpdate;
        this->matrixWorldNeedsUpdate = false;

        //        this->layers = new THREE.Layers();
        this->visible = true;

        this->castShadow = false;
        this->receiveShadow = false;

        this->frustumCulled = true;
        this->renderOrder = 0;

        // this->userData = {};
    }

    void applyMatrix( matrix ) {

        this->matrix.multiplyMatrices( matrix, this->matrix );

        this->matrix.decompose( this->position, this->quaternion, this->scale );

    }

    void setRotationFromAxisAngle( axis, angle ) {

        // assumes axis is normalized

        this->quaternion.setFromAxisAngle( axis, angle );

    }

    void setRotationFromEuler( euler ) {

        this->quaternion.setFromEuler( euler, true );

    }

    void  setRotationFromMatrix( m ) {

        // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

        this->quaternion.setFromRotationMatrix( m );

    }

    void setRotationFromQuaternion( q ) {

        // assumes q is normalized

        this->quaternion.copy( q );

    }

    Object3D& rotateOnAxis(const Vector3& axis,const double& angle)
    {
        Quaternion q1 ;
        q1.setFromAxisAngle( axis, angle );

        this->quaternion.multiply( q1 );
        return *this;
    }

    Object3D& rotateX( const double& angle )
    {
        Vector3 v1( 1, 0, 0 );
        return this->rotateOnAxis( v1, angle );
    }

    Object3D& rotateY( const double& angle )
    {

        Vector3 v1( 0, 1, 0 );
        return this->rotateOnAxis( v1, angle );
    }

    Object3D& rotateZ(const double& angle )
    {
        Vector3 v1( 0, 0, 1 );
        return this->rotateOnAxis( v1, angle );

    }

    Object3D& translateOnAxis(const Vector3&  axis, const double& distance )
    {
        Vector3 v1 ;
        v1.copy( axis ).applyQuaternion( this->quaternion );

        this->position.add( v1.multiplyScalar( distance ) );

        return *this;
    }

    Object3D& translateX(const double& distance )
    {
        Vector3 v1( 1, 0, 0 );
        return this->translateOnAxis( v1, distance );
    }

    Object3D& translateY(const double& distance )
    {

        Vector3 v1 ( 0, 1, 0 );
        return this->translateOnAxis( v1, distance );
    }

    Object3D& translateZ( const double& distance )
    {
        Vector3 v1 ( 0, 0, 1 );
        return this->translateOnAxis( v1, distance );
    }

    Vector3& localToWorld(Vector3& vector ) const
    {
        return vector.applyMatrix4( this->matrixWorld );
    }



    Vector3 & worldToLocal( Vector3& vector ) const
    {
        Matrix4 m1;
        return vector.applyMatrix4( m1.getInverse( this->matrixWorld ) );
    }


    void lookAt() (const Vector3& vector )
    {
        // This routine does not support objects with rotated and/or translated parent(s)
        Matrix4 m1;
        m1.lookAt( vector, this->position, this->up );

        this->quaternion.setFromRotationMatrix( m1 );
    }

//   Object3D& add( object ) {

//        if ( arguments.length > 1 ) {

//            for ( var i = 0; i < arguments.length; i ++ ) {

//                this->add( arguments[ i ] );

//            }

//            return *this;

//        }

//        if ( object == this ) {

//            console.error( "THREE.Object3D.add: object can't be added as a child of itself.", object );
//            return *this;

//        }

//        if ( object instanceof THREE.Object3D ) {

//            if ( object.parent != null ) {

//                object.parent.remove( object );

//            }

//            object.parent = this;
//            object.dispatchEvent( { type: 'added' } );

//            this->children.push( object );

//        } else {

//            console.error( "THREE.Object3D.add: object not an instance of THREE.Object3D.", object );

//        }

//        return *this;

//    }


//    remove( object ) {

//        if ( arguments.length > 1 ) {

//            for ( var i = 0; i < arguments.length; i ++ ) {

//                this->remove( arguments[ i ] );

//            }

//        }

//        var index = this->children.indexOf( object );

//        if ( index != - 1 ) {

//            object.parent = null;

//            object.dispatchEvent( { type: 'removed' } );

//            this->children.splice( index, 1 );

//        }

//    }

    getObjectById( id ) {

        return this->getObjectByProperty( 'id', id );

    }

    getObjectByName( name ) {

        return this->getObjectByProperty( 'name', name );

    }

    getObjectByProperty( name, value ) {

        if ( this[ name ] == value ) return *this;

        for ( var i = 0, l = this->children.length; i < l; i ++ ) {

            var child = this->children[ i ];
            var object = child.getObjectByProperty( name, value );

            if ( object != undefined ) {

                return object;

            }

        }

        return undefined;

    }

    getWorldPosition( optionalTarget ) {

        var result = optionalTarget || new THREE.Vector3();

        this->updateMatrixWorld( true );

        return result.setFromMatrixPosition( this->matrixWorld );

    }

    getWorldQuaternion() {

        var position = new THREE.Vector3();
        var scale = new THREE.Vector3();

        return function ( optionalTarget ) {

            var result = optionalTarget || new THREE.Quaternion();

            this->updateMatrixWorld( true );

            this->matrixWorld.decompose( position, result, scale );

            return result;

        };

    }(),

    getWorldRotation() {

        var quaternion = new THREE.Quaternion();

        return function ( optionalTarget ) {

            var result = optionalTarget || new THREE.Euler();

            this->getWorldQuaternion( quaternion );

            return result.setFromQuaternion( quaternion, this->rotation.order, false );

        };

    }(),

    getWorldScale() {

        var position = new THREE.Vector3();
        var quaternion = new THREE.Quaternion();

        return function ( optionalTarget ) {

            var result = optionalTarget || new THREE.Vector3();

            this->updateMatrixWorld( true );

            this->matrixWorld.decompose( position, quaternion, result );

            return result;

        };

    }(),

    getWorldDirection() {

        var quaternion = new THREE.Quaternion();

        return function ( optionalTarget ) {

            var result = optionalTarget || new THREE.Vector3();

            this->getWorldQuaternion( quaternion );

            return result.set( 0, 0, 1 ).applyQuaternion( quaternion );

        };

    }(),

    raycast() {}

    traverse( callback ) {

        callback( this );

        var children = this->children;

        for ( var i = 0, l = children.length; i < l; i ++ ) {

            children[ i ].traverse( callback );

        }

    }

    traverseVisible( callback ) {

        if ( this->visible == false ) return;

        callback( this );

        var children = this->children;

        for ( var i = 0, l = children.length; i < l; i ++ ) {

            children[ i ].traverseVisible( callback );

        }

    }

    traverseAncestors( callback ) {

        var parent = this->parent;

        if ( parent != null ) {

            callback( parent );

            parent.traverseAncestors( callback );

        }

    }

    updateMatrix() {

        this->matrix.compose( this->position, this->quaternion, this->scale );

        this->matrixWorldNeedsUpdate = true;

    }

    updateMatrixWorld( force ) {

        if ( this->matrixAutoUpdate == true ) this->updateMatrix();

        if ( this->matrixWorldNeedsUpdate == true || force == true ) {

            if ( this->parent == null ) {

                this->matrixWorld.copy( this->matrix );

            } else {

                this->matrixWorld.multiplyMatrices( this->parent.matrixWorld, this->matrix );

            }

            this->matrixWorldNeedsUpdate = false;

            force = true;

        }

        // update children

        for ( var i = 0, l = this->children.length; i < l; i ++ ) {

            this->children[ i ].updateMatrixWorld( force );

        }

    }

    toJSON( meta ) {

        var isRootObject = ( meta == undefined );

        var output = {};

        // meta is a hash used to collect geometries, materials.
        // not providing it implies that this is the root object
        // being serialized.
        if ( isRootObject ) {

            // initialize meta obj
            meta = {
                geometries: {}
                materials: {}
                textures: {}
                images: {}
            };

            output.metadata = {
                version: 4.4,
                type: 'Object',
                generator: 'Object3D.toJSON'
            };

        }

        // standard Object3D serialization

        var object = {};

        object.uuid = this->uuid;
        object.type = this->type;

        if ( this->name != '' ) object.name = this->name;
        if ( JSON.stringify( this->userData ) != '{}' ) object.userData = this->userData;
        if ( this->castShadow == true ) object.castShadow = true;
        if ( this->receiveShadow == true ) object.receiveShadow = true;
        if ( this->visible == false ) object.visible = false;

        object.matrix = this->matrix.toArray();

        //

        if ( this->geometry != undefined ) {

            if ( meta.geometries[ this->geometry.uuid ] == undefined ) {

                meta.geometries[ this->geometry.uuid ] = this->geometry.toJSON( meta );

            }

            object.geometry = this->geometry.uuid;

        }

        if ( this->material != undefined ) {

            if ( meta.materials[ this->material.uuid ] == undefined ) {

                meta.materials[ this->material.uuid ] = this->material.toJSON( meta );

            }

            object.material = this->material.uuid;

        }

        //

        if ( this->children.length > 0 ) {

            object.children = [];

            for ( var i = 0; i < this->children.length; i ++ ) {

                object.children.push( this->children[ i ].toJSON( meta ).object );

            }

        }

        if ( isRootObject ) {

            var geometries = extractFromCache( meta.geometries );
            var materials = extractFromCache( meta.materials );
            var textures = extractFromCache( meta.textures );
            var images = extractFromCache( meta.images );

            if ( geometries.length > 0 ) output.geometries = geometries;
            if ( materials.length > 0 ) output.materials = materials;
            if ( textures.length > 0 ) output.textures = textures;
            if ( images.length > 0 ) output.images = images;

        }

        output.object = object;

        return output;

        // extract data from the cache hash
        // remove metadata on each item
        // and return as array
        function extractFromCache ( cache ) {

            var values = [];
            for ( var key in cache ) {

                var data = cache[ key ];
                delete data.metadata;
                values.push( data );

            }
            return values;

        }

    }

    clone( recursive ) {

        return new this->constructor().copy( this, recursive );

    }

    copy( source, recursive ) {

        if ( recursive == undefined ) recursive = true;

        this->name = source.name;

        this->up.copy( source.up );

        this->position.copy( source.position );
        this->quaternion.copy( source.quaternion );
        this->scale.copy( source.scale );

        this->rotationAutoUpdate = source.rotationAutoUpdate;

        this->matrix.copy( source.matrix );
        this->matrixWorld.copy( source.matrixWorld );

        this->matrixAutoUpdate = source.matrixAutoUpdate;
        this->matrixWorldNeedsUpdate = source.matrixWorldNeedsUpdate;

        this->visible = source.visible;

        this->castShadow = source.castShadow;
        this->receiveShadow = source.receiveShadow;

        this->frustumCulled = source.frustumCulled;
        this->renderOrder = source.renderOrder;

        this->userData = JSON.parse( JSON.stringify( source.userData ) );

        if ( recursive == true ) {

            for ( var i = 0; i < source.children.length; i ++ ) {

                var child = source.children[ i ];
                this->add( child.clone() );

            }

        }

        return *this;

    }

    // private:
    qint64                          id;
    QString                         uuid;
    QString                         name;
    QString                         type;
    Object3D                           parent;     // TODO
    QVector<Object3D>               children;     // TODO
    Vector3                         up;
    Vector3                         position;
    Vector3                         rotation;
    Quaternion                      quaternion;
    double                          scale;
    Matrix4                         modelViewMatrix;
    Matrix3                         normalMatrix;
    bool                            rotationAutoUpdate;
    Matrix4                         matrix;
    Matrix4                         matrixWorld;
    bool                            matrixAutoUpdate;
    bool                            matrixWorldNeedsUpdate;
    Layers                          layers;
    bool                            visible;
    bool                            castShadow;
    bool                            receiveShadow;
    bool                            frustumCulled;
    double                          renderOrder;     // TODO
    QVariant                        userData;    // TODO

    static Vector3                  DefaultUp;
    static bool                     DefaultMatrixAutoUpdate; // true
    static qint64                   Object3DIdCount;       // 0
};



} // namespace three

#endif // THREE_OBJECT3D_H
