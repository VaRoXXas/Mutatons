using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KillingTile : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnTriggerEnter(Collider collider)
    {
        Creature mutaton = collider.GetComponent<Creature>();
        Destroy(mutaton.gameObject);
           
    }
}
