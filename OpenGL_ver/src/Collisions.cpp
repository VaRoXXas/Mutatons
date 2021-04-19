#include "pch.h"
#include "Collisions.h"



void Collisions::PosSizeCalc(glm::mat4& mat)
{
    pos.x = mat[0][0];
    pos.y = mat[1][1];
    pos.z = mat[2][2];
    size.x = mat[0][0] * mat[0][3];
    size.y = mat[1][1] * mat[1][3];
    size.z = mat[2][2] * mat[2][3];

}

bool Collisions::RectCol()
{
	return 1;
}

bool Collisions::Collides(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize)
{
    bool collisionX = onePos.x + oneSize.x >= twoPos.x &&
        twoPos.x + twoSize.x >= onePos.x;
    bool collisionY = onePos.y + oneSize.y >= twoPos.y &&
        twoPos.y + twoSize.y >= onePos.y;
    bool collisionZ = onePos.z + oneSize.z >= twoPos.z &&
        twoPos.z + twoSize.z >= onePos.z;
    return collisionX && collisionY && collisionZ;
}

bool Collisions::Trigger(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize)
{
    if (Collides(onePos, oneSize, twoPos, twoSize))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

glm::vec3 Collisions::Separation(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize)
{
    glm::vec3 separation;
    (onePos.x + oneSize.x - twoPos.x < twoPos.x + twoSize.x - onePos.x) ? separation.x = -(onePos.x + oneSize.x - twoPos.x) : separation.x = twoPos.x + twoSize.x - onePos.x;
    (onePos.y + oneSize.y - twoPos.y < twoPos.y + twoSize.y - onePos.y) ? separation.y = -(onePos.y + oneSize.y - twoPos.y) : separation.y = twoPos.y + twoSize.y - onePos.y;
    (onePos.z + oneSize.z - twoPos.z < twoPos.z + twoSize.z - onePos.z) ? separation.z = -(onePos.z + oneSize.z - twoPos.z) : separation.z = twoPos.z + twoSize.z - onePos.z;
    //if (std::abs(x) < std::abs(y))
    //{
    //    separation.y = 0;
    //}
    //if (std::abs(x) > std::abs(y))
    //{
    //    separation.x = 0;
    //}
    //if (std::abs(x) < std::abs(z))
    //{
    //    separation.z = 0;
    //}
    //if (std::abs(x) > std::abs(z))
    //{
    //    separation.x = 0;
    //}
    //if (std::abs(y) < std::abs(z))
    //{
    //    separation.z = 0;
    //}
    //if (std::abs(y) > std::abs(z))
    //{
    //    separation.y = 0;
    //}
    return separation;
}

//glm::vec3 Collisions::SetSeparation()
//{
//    return Collisions::Separation(glm::vec3 & onePos, glm::vec3 & oneSize, glm::vec3 & twoPos, glm::vec3 & twoSize);
//}

