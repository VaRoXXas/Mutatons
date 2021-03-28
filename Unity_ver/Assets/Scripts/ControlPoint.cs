using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ControlPoint : MonoBehaviour
{
    bool done;
    bool CanBeControlled;
    int counter;
    public ElemField[] fields;

    void Start()
    {
        counter = 0;
        done = false;
    }
    
    void OnTriggerEnter(Collider collider)
    {
        if(!done)
        {
            foreach( ElemField ef in fields)
            {
                if(ef.destroyed == true)
                {
                    counter = counter + 1;
                }
            }
            if( collider.gameObject.tag == "Mutaton" && counter == fields.Length )
            {
                done = true;
                Debug.Log("hit");
                GetComponent<Light>().color = Color.blue;
            } else if(counter != fields.Length)
            {
                counter = 0;
            }
        }

    }
}
