using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElemField : MonoBehaviour
{
    public Material fire;
    public Material water;
    public int element;
    GameObject child;
    void Start()
    {
        child = this.gameObject.transform.GetChild(0).gameObject;
        SetColor(element);
    }

    void OnTriggerEnter(Collider collider)
    {
        Creature mutaton = collider.GetComponent<Creature>();
        if( mutaton != null && mutaton.element == 1 && element == 1)
        {
            Destroy(child);
            Destroy(mutaton.gameObject);
            GetComponent<Collider>().enabled = !GetComponent<Collider>().enabled;
        } else if( mutaton != null && mutaton.element == 2 && element == 2)
        {
            Destroy(child);
            Destroy(mutaton.gameObject);
            GetComponent<Collider>().enabled = !GetComponent<Collider>().enabled;
        } else if( mutaton != null )
        {
            Destroy(mutaton.gameObject);
        }
    }

    void SetColor( int elem )
    {
        if(elem == 1)
        {
            child.GetComponent<MeshRenderer>().material = fire;
        } else if(elem == 2)
        {
            child.GetComponent<MeshRenderer>().material = water;
        }
    }
}
