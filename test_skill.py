import unittest
import skill

class TestSkillCase(unittest.TestCase):
    def test_create_skill(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        self.assertEqual('Test Skill', skill_.name)
        self.assertEqual('10', skill_.base)
        self.assertEqual(skill.SKILL, skill_.type)
        self.assertEqual(skill.ERA_IS1920, skill_.EraMask)
        
    def test_create_empty_skill(self):
        skill_ = skill.Skill()
        self.assertEqual('', skill_.name)
        self.assertEqual('', skill_.base)
        self.assertEqual(skill.NO_TYPE, skill_.type)
        self.assertEqual(skill.ERA_ISNONE, skill_.EraMask)
        
    def test_is_era_pass(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        self.assertTrue(skill_.IsEra(skill.ERA_IS1920))
    
    def test_is_era_pass_all_era(self):
        skill_ = skill.Skill('Test Skill', 10, skill.ERA_ISALL)
        self.assertTrue(skill_.IsEra(skill.ERA_IS1890))
        self.assertTrue(skill_.IsEra(skill.ERA_IS1920))
        self.assertTrue(skill_.IsEra(skill.ERA_IS1990))
        
    def test_is_era_fail(self):
        skill_ = skill.Skill('Test Skill', 10, skill.ERA_IS1920)
        self.assertFalse(skill_.IsEra(skill.ERA_IS1890))
        
def suite():
    return unittest.TestLoader().loadTestsFromTestCase(TestSkillCase)
        
if __name__ == '__main__':
    unittest.main()
    