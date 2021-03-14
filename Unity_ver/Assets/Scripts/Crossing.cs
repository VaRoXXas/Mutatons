using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crossing : MonoBehaviour
{
    [SerializeField] private WorldDirection defaultOpenBridgeDirection;
    private WorldDirection currentOpenBridgeDirection = WorldDirection.NONE;
    [SerializeField] private Bridge northBridge, eastBridge, southBridge, westBridge;
    

    void Start()
    {
        SetBridgeOpen(defaultOpenBridgeDirection);
    }

    void OnTriggerEnter(Collider other)
    {
        creature_AI mutaton = other.gameObject.GetComponent<creature_AI>();
        if(mutaton != null)
        {
            Debug.Log("Mutaton has entered the crossing!");
            mutaton.ChangeMovementDirection(currentOpenBridgeDirection);
        }
    }


    public void SetBridgeOpen(WorldDirection bridgeDirection)
    {
        if(bridgeDirection != currentOpenBridgeDirection)
        {
            if(northBridge != null)
                northBridge.SetOpen(false);
            if(eastBridge != null)
                eastBridge.SetOpen(false);
            if(southBridge != null)
                southBridge.SetOpen(false);
            if(westBridge != null)
                westBridge.SetOpen(false);
            switch (bridgeDirection)
            {
                case WorldDirection.NORTH:
                    if(northBridge != null)
                        northBridge.SetOpen(true);
                    break;
                case WorldDirection.EAST:
                    if(eastBridge != null)
                        eastBridge.SetOpen(true);
                    break;
                case WorldDirection.SOUTH:
                    if(southBridge != null)
                        southBridge.SetOpen(true);
                    break;
                case WorldDirection.WEST:
                    if(westBridge != null)
                        westBridge.SetOpen(true);
                    break;
                default:
                    break;
            }
            currentOpenBridgeDirection = bridgeDirection;
        }
    }
}

public enum WorldDirection
{
    NONE,
    NORTH,
    EAST,
    SOUTH,
    WEST
}