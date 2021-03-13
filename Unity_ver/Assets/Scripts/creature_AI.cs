using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class creature_AI : MonoBehaviour
{

    public float moveForce;
    private bool moveForward;
    public Transform wallDetection;
    void Start()
    {
        
    }

    void Update()
    {
        transform.Translate(Vector2.right * moveForce * Time.deltaTime);
        int layer_mask = LayerMask.GetMask("Wall");
        RaycastHit wallInfo;
            
        if(Physics.Raycast(wallDetection.position, transform.TransformDirection(Vector3.forward),out wallInfo, Mathf.Infinity, layer_mask))
        {
            if (moveForward == true)
            {
                transform.eulerAngles = new Vector3(0, -180, 0);
                moveForward = false;
            }
            else
            {
                transform.eulerAngles = new Vector3(0, 0, 0);
                moveForward = true;
            }
        }
        

    }
}
