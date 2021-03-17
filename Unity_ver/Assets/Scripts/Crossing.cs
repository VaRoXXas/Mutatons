using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crossing : MonoBehaviour
{
    [SerializeField] private WorldDirection defaultOpenBridgeDirection;
    private WorldDirection currentOpenBridgeDirection = WorldDirection.NONE;
    [SerializeField] private Bridge northBridge, eastBridge, southBridge, westBridge;
    public Material[] material;
    Renderer rend;
    public bool isClickable;// to group crossing with bridges and without bridges
    

    void Start()
    {
        SetBridgeOpen(defaultOpenBridgeDirection);
        currentOpenBridgeDirection = defaultOpenBridgeDirection;

        if (isClickable)
        {
            rend = GetComponent<Renderer>();
            rend.enabled = true;

            switch (defaultOpenBridgeDirection)
            {
                case WorldDirection.NORTH:
                    rend.sharedMaterial = material[0];
                    break;
                case WorldDirection.SOUTH:
                    rend.sharedMaterial = material[1];
                    break;
                case WorldDirection.EAST:
                    rend.sharedMaterial = material[2];
                    break;
                case WorldDirection.WEST:
                    rend.sharedMaterial = material[3];
                    break;

            }
        }
        
    }

    void OnTriggerEnter(Collider other)
    {
        Creature mutaton = other.gameObject.GetComponent<Creature>();
        if(mutaton != null)
        {
            Debug.Log("Mutaton has entered the crossing!");
            mutaton.ChangeMovementDirection(currentOpenBridgeDirection);
        }
    }

    void OnMouseDown()
    {
        if(isClickable)
        {
            switch (currentOpenBridgeDirection)
            {
                case WorldDirection.NORTH:
                    currentOpenBridgeDirection = WorldDirection.SOUTH;
                    rend.sharedMaterial = material[1];
                    break;
                case WorldDirection.SOUTH:
                    currentOpenBridgeDirection = WorldDirection.NORTH;
                    rend.sharedMaterial = material[0];
                    break;
                case WorldDirection.WEST:
                    currentOpenBridgeDirection = WorldDirection.EAST;
                    rend.sharedMaterial = material[2];
                    break;
                case WorldDirection.EAST:
                    currentOpenBridgeDirection = WorldDirection.WEST;
                    rend.sharedMaterial = material[3];
                    break;

            }
            SetBridgeOpen(currentOpenBridgeDirection);
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