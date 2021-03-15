using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LabPoint : MonoBehaviour
{
    public int element;
    // 1 - fire, 2 - water,
    void Start()
    {
        Debug.Log(gameObject.name);
        SetColor(element);
    }
    
    void OnTriggerEnter(Collider collider)
    {
        Creature mutaton = collider.GetComponent<Creature>();
        if( mutaton != null && element == 1)
        {
            Debug.Log("hit");
            mutaton.element = 1;
        } else if( mutaton != null && element == 2)
        {
            Debug.Log("hit");
            mutaton.element = 2;
        }
    }
    
    void SetColor( int elem )
    {
        if(elem == 1)
        {
            GetComponent<Light>().color = Color.red;
        } else if(elem == 2)
        {
            GetComponent<Light>().color = Color.blue;
        }
    }
}
