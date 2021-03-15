using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ControlPoint : MonoBehaviour
{
    bool done;

    void Start()
    {
        done = false;
        Debug.Log(gameObject.name);
    }
    
    void OnTriggerEnter(Collider collider)
    {
        if(!done)
        {
            if( collider.gameObject.tag == "Mutaton" )
            {
                done = true;
                Debug.Log("hit");
                GetComponent<Light>().color = Color.blue;
            }
        }

    }
}
