using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Creature : MonoBehaviour
{

    public float moveForce;
    private bool moveForward;
    public Transform wallDetection;
    public int element;
    private WorldDirection newDirection;
    private WorldDirection currentDirection;
    //private int turnAngle = 0;
    void Start()
    {
        moveForward = true;
        element = 0;
        newDirection = WorldDirection.EAST;
        currentDirection = WorldDirection.NORTH;
    }

    void Update()
    {
        transform.Translate(Vector3.forward * moveForce * Time.deltaTime);
        int layer_mask = LayerMask.GetMask("Wall");
        RaycastHit wallInfo;
       
        if (Physics.Raycast(wallDetection.position, transform.TransformDirection(Vector3.forward),out wallInfo, 0.1f, layer_mask))
        {

            switch (currentDirection)
            {
                case WorldDirection.NORTH:
                    newDirection = WorldDirection.SOUTH;
                    ChangeMovementDirection(newDirection);
                    currentDirection = newDirection;
                    Debug.Log("south");
                    break;
                case WorldDirection.EAST:
                    newDirection = WorldDirection.WEST;
                    ChangeMovementDirection(newDirection);
                    currentDirection = newDirection;
                    Debug.Log("west");
                    break;
                case WorldDirection.SOUTH:
                    newDirection = WorldDirection.NORTH;
                    ChangeMovementDirection(newDirection);
                    currentDirection = newDirection;
                    Debug.Log("north");
                    break;
                case WorldDirection.WEST:
                    newDirection = WorldDirection.EAST;
                    ChangeMovementDirection(newDirection);
                    currentDirection = newDirection;
                    Debug.Log("east");
                    break;
                default:
                    break;
            }
            /* switch (currentDirection)
             {
                 case WorldDirection.NORTH:
                     newDirection = WorldDirection.SOUTH;
                     turnAngle = -180;
                     Debug.Log("south");
                     break;
                 case WorldDirection.EAST:
                     newDirection = WorldDirection.WEST;
                     turnAngle = -90;
                     Debug.Log("west");
                     break;
                 case WorldDirection.SOUTH:
                     newDirection = WorldDirection.NORTH;
                     turnAngle = 0;
                     Debug.Log("north");
                     break;
                 case WorldDirection.WEST:
                     newDirection = WorldDirection.EAST;
                     turnAngle = 90;
                     Debug.Log("east");
                     break;
                 default:
                     break;
             }

             transform.eulerAngles = new Vector3(0, turnAngle, 0);
              */

        }
        
        if(element != 0)
        {
            Debug.Log("changed element");
        }
    }

    // Method used by crossings.
    public void ChangeMovementDirection(WorldDirection newDirection)
    {
        switch (newDirection)
        {
            case WorldDirection.NORTH:
                transform.eulerAngles = new Vector3(0, 0, 0);
                currentDirection = newDirection;
                Debug.Log("north");
                break;
            case WorldDirection.EAST:
                transform.eulerAngles = new Vector3(0, -270, 0);
                currentDirection = newDirection;
                Debug.Log("east");
                break;
            case WorldDirection.SOUTH:
                transform.eulerAngles = new Vector3(0, -180, 0);
                currentDirection = newDirection;
                Debug.Log("south");
                break;
            case WorldDirection.WEST:
                transform.eulerAngles = new Vector3(0, -90, 0);
                currentDirection = newDirection;
                Debug.Log("west");
                break;
            case WorldDirection.NONE:
                
                Debug.Log("kurwa");
                break;
            default:
                break;
        }

        // Mutaton's movement change implementation.
    }
}
